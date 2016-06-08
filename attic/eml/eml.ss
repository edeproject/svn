(define *widget-storage* '())

(define *error-hook* println)

(define (widget-storage-add! lst)
  (let1 lst (partition 2 lst)
    (set! *widget-storage* (cons lst *widget-storage*)) ) )

;; Get object details by searching by object ID string
(define (widget-get-id str)
  (let1 lst *widget-storage*
    (let loop ()
	  (when-not (empty? lst)
	    (let3 sl (car lst)
			  v  (assoc ':id sl)
			  v  (if v (cadr v)) ;; get value it holds
		  (if (and v
				   (string=? v str))
			;; return full list
			  (begin (println sl)
		    sl)
			;; next
			(begin
			  (set! lst (cdr lst))
			  (loop) ) ) ) ) ) ) )

;;; globally visible

;; Return all visible ID's
(define (eml-get-all-ids)
  (let loop ([lst *widget-storage*])
	(if (empty? lst)
	  '()
	  (begin
		(let3 ll (car lst)
			  v  (assoc ':id ll)
			  v  (if v (cadr v))
		  (if v
		    (cons v (loop (cdr lst)))
			(loop (cdr lst)) ) ) ) ) ) )

(define-macro (eml-get id key)
  (let ([vv (gensym)])
	`(let ([,vv (assoc ',key (widget-get-id ,id))])
	   (if ,vv
	     (cadr ,vv)
		 #f) ) ) )

(define (eml-get-as-string id key)
  (println "===> " id " : " (string? id))
  (println "===> " key " : " (string? key))
  (let2 key (string->symbol key)
        vv  (assoc key (widget-get-id id))
    (if vv
	  (cadr vv)
	  #f ) ) )
			  
(define-macro (Widget . args)
  `(widget-storage-add! ',args))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;(Widget :type  button
;		:id    "xbutton"
;		:label _"Save"
;		:x     10
;		:y     10
;		:w     90
;		:h     25
;		:callback print-me)
;
;(Widget :type    window
;		:subtype edelib
;		:id      "xwindow"
;		:add     (list "xbutton" "xlabel"))
;
;(println (widget-assoc (widget-get-id "xwindow") :add))
;(println (widget-get-all-ids))
;
;(println (widget-assoc '((:foo 4) (:boo 5)) :boo))
