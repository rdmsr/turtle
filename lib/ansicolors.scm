;; Reimplementation of https://github.com/rotty/guile-lib/blob/master/src/term/ansi-color.scm
;; Thanks to them

(define-module (ansicolors)
     #:export  (color
                colorize-string)
     #:use-module (srfi srfi-1)
     #:use-module (srfi srfi-13)) 

(define ansi-color-tables
  (let ((table (make-hash-table 23)))
    (hashq-set! table 'CLEAR "0")
    (hashq-set! table 'RESET "0")
    (hashq-set! table 'BOLD  "1")
    (hashq-set! table 'DARK  "2")
    (hashq-set! table 'UNDERLINE "4")
    (hashq-set! table 'UNDERSCORE "4")
    (hashq-set! table 'BLINK "5")
    (hashq-set! table 'REVERSE "6")
    (hashq-set! table 'CONCEALED "8")
    (hashq-set! table 'BLACK "30")
    (hashq-set! table 'RED "31")
    (hashq-set! table 'GREEN "32")
    (hashq-set! table 'YELLOW "33")
    (hashq-set! table 'BLUE "34")
    (hashq-set! table 'MAGENTA "35")
    (hashq-set! table 'CYAN "36")
    (hashq-set! table 'WHITE "37")
    (hashq-set! table 'BG-BLACK "40")
    (hashq-set! table 'BG-RED "41")
    (hashq-set! table 'BG-GREEN "42")
    (hashq-set! table 'BG-YELLOW "43")
    (hashq-set! table 'BG-BLUE "44")
    (hashq-set! table 'BG-MAGENTA "45")
    (hashq-set! table 'BG-CYAN "46")
    (hashq-set! table 'BG-WHITE "47")
    table))

(define-public (color . lst)
  (let ((color-list 
         (remove not 
                 (map (lambda (color) (hashq-ref ansi-color-tables color))
                      lst))))
    (if (null? color-list)
        ""
        (string-append 
         (string #\esc #\[)
         (string-join color-list ";" 'infix)
         "m"))))
  
(define-public (colorize-string str . color-list)
  (string-append
   (apply color color-list)
   str
   (color 'RESET)))
