(define-module (turtlerc)
  #:use-module (ansicolors)) ;; use the ansicolors module

(define-public prompt (string-concatenate (list (colorize-string "λ " 'GREEN 'BOLD) (color 'RESET)))) ;; Green lambda

(define-public failed-prompt (string-concatenate (list (colorize-string "λ " 'RED 'BOLD) (color 'RESET)))) ;; Red lambda

(display (string-concatenate (list "Welcome to " (color 'GREEN 'BOLD) "turtle" (color 'RESET) ", " (color 'CYAN 'BOLD) (getenv "USER") (color 'RESET) ".\nA little shell for " (color 'BLUE 'BOLD) "scheme " (color 'RESET) "enthusiasts.\n"))) ;; Welcome message
