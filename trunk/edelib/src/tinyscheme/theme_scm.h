/* Generated with gen-c-string.sh. Do not edit this file, edit .scm file */
static const char theme_scm_content[] =
    " (define edelib-dir-separator      #f) "
    " (define private:theme.search-path #f) "
    " (define private:theme.author #f) "
    " (define private:theme.sample #f) "
    " (define private:theme.name   #f) "
    " (define private:theme.styles '()) "
    " (define (theme.path-resolve item)  "
    "  (if (and edelib-dir-separator "
    "           private:theme.search-path)  "
    "   (string-append private:theme.search-path edelib-dir-separator item) "
    "    item  "
    " )) "
    " (define (include file) "
    "  (load (theme.path-resolve file)) "
    " ) "
    " (define (theme.print msg . args) "
    "  (display msg) "
    "  (for-each (lambda (x) (display x)) args) "
    " ) "
    " (define (theme.warning msg . args) "
    "  (display \"*** ede theme warning: \") "
    "  (display msg) "
    "  (for-each (lambda (x) (display x)) args) "
    " ) "
    " (define (theme.error msg . args) "
    "  (newline) "
    "  (display \"*** ede theme error: \") "
    "  (display msg) "
    "  (for-each (lambda (x) (display x)) args) "
    "  (newline) "
    " ) "
    " (define *error-hook* theme.error) "
    " (define (theme.author a) "
    "  (set! private:theme.author a)) "
    " (define (theme.author-get) "
    "  private:theme.author) "
    " (define (theme.sample s) "
    "  (set! private:theme.sample (theme.path-resolve s)) "
    " ) "
    " (define (theme.sample-get) "
    "   private:theme.sample) "
    " (define (theme.name n) "
    "  (set! private:theme.name n)) "
    " (define (theme.name-get) "
    "  private:theme.name) "
    " (define-macro (theme.style name . body)  "
    "  `(set! private:theme.styles (cons (list ,name ',@body) private:theme.styles)) "
    " ) "
    " (define (theme.style-get name) "
    "  (let ((ret '())) "
    "   (for-each (lambda (x) "
    "              (if (string=? name (car x)) "
    "               (set! ret (cadr x)))) "
    "    private:theme.styles "
    "   ) "
    "   ret "
    " )) "
;