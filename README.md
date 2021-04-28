# Turtle
<div align="center">
	<blockquote>
        A little shell for scheme enthusiasts 🐢 
	</blockquote>
</div>

Turtle is a little shell that is written in C and is extensible in scheme/guile.


## Configuration
Here is the default configuration:

```scheme
(define-module (turtlerc))
(define-public prompt "\x1b[1;32mλ \x1b[0m")

(format #t "Welcome to\x1b[1;32m turtle, \x1b[1;36m~a\x1b[0m.\nA little shell for \x1b[1;34mscheme\x1b[0m enthusiasts. \n" (getenv "USER"))
```

![image](https://user-images.githubusercontent.com/54687179/116449836-96608c00-a828-11eb-8f72-9c69e0378b3d.png)


## Building
You need guile and readline for this shell to work.

Then run `make && make install`