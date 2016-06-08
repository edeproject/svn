#!./newlisp -c
# -*- mode: lisp -*-

;; page template dir (local)
(setq *page-template-dir* "./templates")

;; location for site media (URL)
(setq *page-site-media* "sitemedia")

;; directory where things are cached
(setq *cache-dir* "/tmp")

;; from where to fetch url
(setq *feed-url* "http://sanelz.blogspot.com/feeds/posts/default")

;; news file
(setq *page-news* (format "%s/%s" *page-template-dir* "news.txt"))

;; main page template
(setq *page-template* (format "%s/%s" *page-template-dir* "index.html"))

;; time when cache should be refreshed (in secs.)
(setq *feed-cache-refresh-time* 3000)

;; fetched titles from the feed
(setq *feed-titles* '())

;; fetched links for each feed
(setq *feed-links* '())

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; thrush macros from Clojure (fount on newlisp forum, (c) johu)
(define-macro (->>:->> E form)
  (letex (_func
          (if $args (cons '->> (cons (list '->> E form) $args))
            (list? form) (push E form -1)
            (list form E)))
         _func))

(define-macro (->:-> E form)
  (letex (_func
          (if $args (cons '-> (cons (list '-> E form) $args))
            (list? form) (push E form 1)
            (list form E) ) )
         _func ) )

;; Simple template engine; all code between "<% ... %>" will be marked for evaluation
;; make sure to use <% (print xxx) %> so value can be printed; stolen from cgi.lsp.
;; It also supports including another template files with '<%include file.html %>' tag
(define OPEN_TAG  "<%")
(define CLOSE_TAG "%>")

(define (eval-template str (dir *page-template-dir*))
  (letn (start   (find OPEN_TAG str)
         end     (find CLOSE_TAG str))

    (while (and start end)
      (print (slice str 0 start))

      (letn (to-eval     (slice str (+ start 2) (- end start 2))
             has-include (starts-with to-eval "include") 
             subtempl    (if has-include
                           (trim (slice to-eval 7)) ) )
        (if has-include
          (->> (format "%s/%s" *page-template-dir* subtempl) read-file eval-template)
          ;; else just eval string
          (eval-string to-eval))

        ;; continue loop
        (setf str   (slice str (+ end 2))
              start (find OPEN_TAG str)
              end   (find CLOSE_TAG str) ) ) )
    (print str)
) )

;; load file from 'template', evaluate it and store content in 'result-file'
(define (eval-template-to-file template result-file)
  ;; use 'device' to redirect stdout inside file
  (device (open result-file "write"))
  (->> template read-file eval-template)
  (close (device)))

;; handle 
(define (include-template f)
  (->> (append *page-template-dir* "/" f)
       read-file
       eval-template
       print))

;; handy function for printing from templates; if variable is nil, it will print nothing
(define (tprint var)
  (if var
    (print var)))

(define (site-media-str object)
  (if object
    (format "%s/%s" *page-site-media* object)))

;; so css, js and etc. locations can be configurable
(define (site-media object)
  (tprint (site-media-str object)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; blogger feed parser
(define (valid-list? lst)
  (and (list? lst)
       (not (empty? lst)) ) )

;; check if entry category has given tag
(define (has-category-tag? content indexes tag)
  (catch
    (dolist (item (nth indexes content))
      (when (and (valid-list? item)
                 (= "category" (nth 0 item)))

        (letn (term-val (->> item (nth 1) (assoc "term"))
               val      (if term-val
                          (nth 1 term-val)))
          (if (= val tag)
            (throw true)
) ) ) ) ) )

;; urls has '("rel" "alternate")' tag/value set
(define (extract-url entry)
  (catch
    (letn (links  (ref-all "link" entry))
      (dolist (link links)
        (letn (tmp link
               _   (pop tmp -1)
               lst (nth tmp entry)
               lst (nth 1 lst)
               url (if (find '("rel" "alternate") lst)
                     (->> lst
                          (assoc "href")
                          (nth 1) ) ) )
          (if url
            (throw url)
) ) ) ) ) )

(define (feed-entry-from-content content)
  (catch
    (letn (_       (xml-type-tags nil 'cdata nil nil) ;; setup xml parser
           xmlfile (xml-parse content))

      ;; get list of positions where 'entry' exists where each next position index in sublist
      ;; e.g. (0 19 0) -> first element, then 19th el. in sublist, then first in sublist of that list
      ;; but we are interested in all except last, as we are targeting whole 'entry' tree
      (dolist (entries (ref-all "entry" xmlfile))
        (letn (tmp  entries
               _    (pop tmp -1)) ;; pop will modify the list returning last element

          (when (has-category-tag? xmlfile tmp "EDE")
            (letn (entry  (nth tmp xmlfile)
                   title  (->> entry (assoc "title") (nth 2))
                   url    (extract-url entry))

              ;; return list
              (if title
                (throw (list title url))
) ) ) ) ) ) ) )

(define (download-feed save-to tmp)
  (->> (get-url *feed-url* 5000 "User-Agent: Mozilla/4.0" 3000)
       (write-file tmp))
  (rename-file tmp save-to))

(define (fetch-blogger-feed)
  (letn (cached-path     (format "%s/%s" *cache-dir* "/feed-cache")
         cached-path-tmp (format "%s-%u" cached-path (rand 10000))
         stat            (file-info cached-path -1)
         diff            (if stat
                             (- (date-value) stat)))

    ;; use Cilk-like API to fetch feed in background thread; at first showing, the feed will not be
    ;; visible causing nil to be returned, which is expected behavior
    (when (or (not diff)
              (> diff *feed-cache-refresh-time*))
      (spawn 'p1
             (download-feed cached-path cached-path-tmp)))

    ;; read file
    (setq feed-content (read-file cached-path))

    (if (or (= feed-content nil)
            (starts-with feed-content "ERR: "))
      nil
      (feed-entry-from-content feed-content)
) ) )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; news reader and parser
;; it has simple xml-like markup and all except 'document' and 'news' tags are considered html elements
(define (news->html path)
  (letn (content (read-file path))
    (replace "<document>" content "<div class=\"newscontent\">")
    (replace "</document>" content "</div><!-- newscontent -->")
    (replace "</news>" content "</div> <!-- news ends -->")
    (replace "<news title=\"(.*?)\">" content (append "<div class=\"news\"><h2>" $1 "</h2>") 0 1)

    (println content)
) )

(define (news->first-title path)
  (catch
    (letn (fd (open path "read"))
      (when fd
        (while (read-line fd)
          (setq regexmtch (regex "<news title=\"(.*?)\">" (current-line) 1))
          (when regexmtch
            (throw (nth 3 regexmtch) ) ) )
        (close fd))
      nil
) ) )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (display-error)
  (println "<html><body>")
  (println "<h2>Fatal: Unable to find a page template</h2>")
  (print "If developers didn't notice this error, please take a few minutes and report it at ")
  (println "<a href=\"http://bugs.equinox-project.org\">http://bugs.equinox-project.org</a>. Thank you!")
  (println "</body></html>") )

(define (generate-page template-file)
  (letn (fpath       (format "%s/%s" *page-template-dir* template-file)
         rnd         (rand 10000)
         processed   (format "%s/ede-template-%u.html" *cache-dir* rnd))

    (eval-template-to-file fpath processed)
    (setq page (read-file processed))
    (delete-file processed)

    (if page
      (println page)
      (display-error) ) ) )

;; front page is specific
(define (generate-front-page news.txt)
  (letn (rnd         (rand 10000)
         processed   (format "%s/ede-template-%u.html" *cache-dir* rnd)
         latest-news (news->first-title news.txt))

    (eval-template-to-file *page-template* processed)
    (setq page (read-file processed))
    (delete-file processed)

    ;; replace our tags
    (when page
      (letn (f    (fetch-blogger-feed)
             item (if f
                    (nth 0 f) 
                    "(Unable to fetch feed)")
             link (if f
                    (nth 1 f)
                    "#"))  

        (replace "<?devlog-item?>"      page item)
        (replace "<?devlog-item-link?>" page link)
        (replace "<?news-title?>"       page latest-news)
        (println page) ) )

    (unless page
      (display-error) ) ) )

;;; main

;; init random generator
(seed (time-of-day))

;; required headers
(println "Cache-Control: max-age=3600")
(println "Content-Type: text/html\n")

(setq *arg* (-> 3 main-args))

(setq elapsed
      (time
        (case *arg*
          ("news"
            (generate-page "news.html"))
          ("screenshots"
            (generate-page "screenshots.html"))
          (true
            (generate-front-page *page-news*) ) ) ) )

(println "<!-- generated in " elapsed " ms with newLISP ;) -->")

;;; wait to all processes to be completed
(sync 10000)
(exit)
