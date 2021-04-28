(define-module (turtlerc))
(define-public prompt "\x1b[1;32mλ \x1b[0m")

(format #t "Welcome to\x1b[1;32m turtle, \x1b[1;36m~a\x1b[0m.\nA little shell for \x1b[1;34mscheme\x1b[0m enthusiasts. \n" (getenv "USER"))
