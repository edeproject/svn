;;
;; A simple demo to dump current XSETTINGS database. Uses:
;;
;; (query-xsettings-all)      - list of all known keys
;; (query-xsettings key)      - get value for this key
;; (query-xsettings-type key) - get a value type for this key

(define (format-space item)
 (let ([diff (- 30 (string-length item))])
  (make-string diff)
 )
)

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
