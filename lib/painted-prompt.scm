(define-module (turtle painted-prompt)
  #:use-module (ice-9 popen)
  #:use-module (ice-9 rdelim))

(define git-branch-name "")
(define-public git-branch-default "")


; Function taken from Guile 3.0
(define (string-replace-substring str substring replacement)
(let ((sublen (string-length substring)))
  (with-output-to-string
       (lambda ()
          (let lp ((start 0))
        (cond
              ((string-contains str substring start)
            => (lambda (end)
                     (display (substring/shared str start end))
                    (display replacement)
                 (lp (+ end sublen))))
              (else
               (display (substring/shared str start)))))))))

(define get-git-branch
(lambda ()
  (let* ((port (open-input-pipe "git rev-parse --abbrev-ref HEAD 2> /dev/null"))
	 (str  (read-line port)))
    (close-pipe port)
    (if (not (eof-object? str))
	(set! git-branch-name str)
        (set! git-branch-name git-branch-default)))
  git-branch-name))

(define-public prompt-git-branch (lambda () (get-git-branch)))


(define-public prompt-user (lambda() (getenv "USER")))
(define-public prompt-dir (lambda() (string-replace-substring (getcwd) (getenv "HOME") "~")))
