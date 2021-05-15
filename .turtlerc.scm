(define-module (turtlerc)
#:use-module(turtle ansicolors)
#:use-module(turtle painted-prompt)) ; use the painted-prompt (get it? painted turtle) module for prompt customization (git and other stuff)

(define git-module (lambda () (if (not (equal? (prompt-git-branch) "" ))	  ; If is in git repo
				  (colorize-string (string-concatenate (list "on שׂ " (prompt-git-branch) "\n")) 'YELLOW 'BOLD) ; return branch
				  ""))) ; else, return nothing

(define user-module (lambda () (colorize-string (prompt-user) 'BLUE 'BOLD))) ; Username
(define dir-module (lambda () (colorize-string (prompt-dir) 'CYAN 'BOLD))) ; Current dir

(define (prompt return_code)
  (define sign "")
  (if (eq? return_code 0) ; if return_code == 0
      (set! sign (colorize-string "λ " 'GREEN 'BOLD)) ; Make the prompt green
      (set! sign (colorize-string "λ " 'RED 'BOLD))) ; Else, make it red
  (string-concatenate (list (git-module) (user-module) " " (dir-module) " " sign (color 'RESET))))


(display (string-concatenate (list "Welcome to " (color 'GREEN 'BOLD) "turtle" (color 'RESET) ", " (color 'CYAN 'BOLD) (getenv "USER") (color 'RESET) ".\nA little shell for " (color 'BLUE 'BOLD) "scheme " (color 'RESET) "enthusiasts.\n"))) ;; Welcome message
