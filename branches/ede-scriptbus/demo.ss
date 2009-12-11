;;
;; A simple demo to dump current XSETTINGS database and window list. 
;;
;; (query-xsettings-all)      - list of all known keys
;; (query-xsettings key)      - get value for this key
;; (query-xsettings-type key) - get a value type for this key
;;
;; (wm-get-window-list)       - get id of all available windows
;; (wm-get-window-title id)   - get title of the given id

(define (format-space item)
 (let ([diff (- 30 (string-length item))])
  (make-string diff)
 )
)

(display "\n*** list XSETTINGS keys and values ***\n")

(define all (query-xsettings-all))

(for-each (lambda (x)
            (display x)
            (display (format-space x))
            (display (query-xsettings x))
            (display " [")
            (display (query-xsettings-type x))
            (display "]")
            (newline)
          )
 all)

(display "\n*** list visible windows ***\n")

(set! all (wm-get-window-list))

(for-each (lambda (id)
            (display (wm-get-window-title id))
            (newline)
          )
 all)
