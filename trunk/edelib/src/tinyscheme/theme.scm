;; vim:ft=scheme:expandtab
;; Theming stuff for edelib and ede. This file should be initialized after init.scm.
;; NOTE: keep the comments at the beginning of the line, so gen-c-string.sh can easily strip them

;; globals set by Theme class
(define edelib-dir-separator      #f)
(define private:theme.search-path #f)

;; globals read by edelib Theme class
(define private:theme.author #f)
(define private:theme.sample #f)
(define private:theme.name   #f)
(define private:theme.styles '())

;; helper to convert given list to paired list
;; it is not efficient as first I'm converting things in vector, but didn't had enough inspiration
;; for smarter solution
;;(define (pair-items lst)
;;  (let* ([vec (list->vector lst)]
;;         [sz  (vector-length vec)]
;;         [ret '()])
;;
;;    (let loop ([i 0]
;;               [j 1])
;;
;;      (when (< j sz)
;;        (set! ret 
;;          (cons 
;;            (list (vector-ref vec i) 
;;                  (vector-ref vec j))
;;            ret ))
;;        ;; next iteration
;;        (loop (+ i 2)
;;              (+ j 2)
;;    ) ) ) 
;;    ;; reverse things as 'cons' put them in reverse order
;;    (reverse ret)
;;) )

;; Resolve path of theme specific items like local files, images or sounds.
(define (theme.path-resolve item) 
 (if (and edelib-dir-separator
          private:theme.search-path) 
  (string-append private:theme.search-path edelib-dir-separator item)
  ;; else
   item 
))

;; We make a difference between (include) and (load). (include) will try
;; to load resolved path so absoluting path is not necessary.
(define (include file)
 (load (theme.path-resolve file))
)

;; some printing functions
(define (theme.print msg . args)
 (display msg)
 (for-each (lambda (x) (display x)) args)
)

(define (theme.warning msg . args)
 (display "*** ede theme warning: ")
 (display msg)
 (for-each (lambda (x) (display x)) args)
)

(define (theme.error msg . args)
 (newline)
 (display "*** ede theme error: ")
 (display msg)
 (for-each (lambda (x) (display x)) args)
 (newline)
)

;; use 'theme.error' as default report function
(define *error-hook* theme.error)

(define (theme.author a)
 (set! private:theme.author a))

(define (theme.author-get)
 private:theme.author)

(define (theme.sample s)
 (set! private:theme.sample (theme.path-resolve s))
)

(define (theme.sample-get)
  private:theme.sample)

(define (theme.name n)
 (set! private:theme.name n))

(define (theme.name-get)
 private:theme.name)

;; define a new style and store it in global style list
(define-macro (theme.style name . body) 
 `(set! private:theme.styles (cons (list ,name ',@body) private:theme.styles))
)

;; called from Theme.cpp to find and return given style name
(define-with-return (theme.style-get name)
 (for-each (lambda (x)
             (if (string=? name (car x))
              (return (cadr x)) ))
 private:theme.styles
))
