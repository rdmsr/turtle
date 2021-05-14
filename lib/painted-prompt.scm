(define-module (turtle painted-prompt)
  #:export (git-branch git-branch-default)
  #:use-module (ice-9 popen)
  #:use-module (ice-9 rdelim))

(define git-branch-name "")
(define git-branch-default "")

(define get-branch
(lambda ()
  (let* ((port (open-input-pipe "git rev-parse --abbrev-ref HEAD 2> /dev/null"))
	 (str  (read-line port)))
    (close-pipe port)
    (if (not (eof-object? str))
	(set! git-branch-name str)
        (set! git-branch-name git-branch-default)))
  git-branch-name))

(define-public git-branch (lambda () (get-branch)))
