(define-module (turtlerc)
  #:use-module (turtle ansicolors)
  #:use-module (turtle painted-prompt)) ;; use the painted-prompt module for prompt customization (git and other stuff)

(set! git-branch-default "Not a git repo")

(define git-module (lambda () (colorize-string (string-concatenate (list "שׂ " (git-branch) "\n")) 'YELLOW 'BOLD)))
(define user-module (lambda () (colorize-string "%u " 'BLUE 'BOLD)))
(define dir-module (lambda () (colorize-string "%d " 'CYAN 'BOLD)))

(define lambda (colorize-string "λ " 'GREEN 'BOLD))
(define failed-lambda (colorize-string "λ " 'RED 'BOLD))

(define-public prompt (string-concatenate (list (git-module) (user-module) (dir-module) lambda (color 'RESET)))) ;; Green lambda

(define-public failed-prompt (string-concatenate (list (git-module) (user-module) (dir-module) failed-lambda (color 'RESET)))) ;; Red lambda

(display (string-concatenate (list "Welcome to " (color 'GREEN 'BOLD) "turtle" (color 'RESET) ", " (color 'CYAN 'BOLD) (getenv "USER") (color 'RESET) ".\nA little shell for " (color 'BLUE 'BOLD) "scheme " (color 'RESET) "enthusiasts.\n"))) ;; Welcome message
