;; Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
;;
;; This Program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public
;; License as published by the Free Software Foundation; either
;; version 3 of the License, or (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;;
;; For more information on the GPL, please go to:
;; http://www.gnu.org/copyleft/gpl.html

;; Need to install modules with chicken:
;;
;; chicken-setup opengl glut syntax-case pos args

(require-extension srfi-1 srfi-4 gl glut posix srfi-18 srfi-12 syntax-case pos)

; some linear algebra functions
(define (square x)
  (* x x))
(define (cube x)
  (* x x x))
(define (magnitude vec)
  (sqrt (apply + (map square vec))))

(define (normalize vec)
  (let ((mag (magnitude vec)))
    (map (lambda (x) (/ x mag)) vec)))

(define (dot . vecs)
  (apply + (apply map * vecs)))


(define pi (* 2 (asin 1)))
(define (deg2rad x) (* pi (/ x 180)))

(define horizdilution 1.5)

(define curpos '(0 0 0 0))

(define points '())
(define triangles '())
(define  (trianglegrid-addpoint x y z mag)
  (let ((p (list x y z mag)))
	(set! points (cons p points)))
  )

(define colorspace-min 1)
(define colorspace-max 0)
(define (recalc-colorspace)
  (for-each (lambda (p)
	      (let ((c (fourth p)))
		(if (< c colorspace-min)
		    (set! colorspace-min c))
		(if (> c colorspace-max)
		    (set! colorspace-max c))))
	    points))
		    
(define (colorspace mag)
  (let ((diff (- colorspace-max colorspace-min)))
    (if (= diff 0) (set! diff .01))
    (let ((n (+ (/ (- mag (/ (+ colorspace-min colorspace-max) 2))
		   diff) .5)))
      (map (lambda (i) (- 1 (* 2 (abs (- i n))))) '(0 .5 1)))))

(define (make-gl-point p)
  (apply gl:Color3f (colorspace (cadddr p)))
  (gl:Vertex3f (car p) (cadr p) (caddr p)))

(define  (trianglegrid-draw)
;  (gl:Begin gl:POINTS)
;  (for-each make-gl-point points)
;  (gl:End)
  (recalc-colorspace)
  (for-each (lambda (point)	      
	      (pos-draw point .1 (colorspace (cadddr point))))
	    points)

)

(define point-pos car)
(define point-color cadr)
(define (new-point pos mag)
  (list pos mag))

(define (draw-point point)
  (let ((c (colorspace (point-color point))))
    (gl:Color3f c c c))
  (gl:Vertex3fv (point-pos point)))


; make syntax for various opengl features to make them nicer
(define-syntax glBegin (syntax-rules ()
			 ((glBegin val body ...)
			  (begin (gl:Begin val) body ... (gl:End)))))

(define-syntax glLetMatrix (syntax-rules ()
			 ((glLetMatrix body ...)
			  (begin (gl:PushMatrix) body ... (gl:PopMatrix)))))

(define-syntax glLetEnable (syntax-rules ()
			     ((glLetEnable (val ...) body ...)
			      (begin (begin (gl:Enable val) ...)
				     (begin body ...)
				     (begin (gl:Disable val) ...)))))

(define (draw-triangles triangles)
  (let ((x (apply min points))
	(y (apply max points)))
	   
  (glBegin gl:TRIANGLES
	   (for-each (lambda (triangle)
		       (draw-point (car point))
		       (draw-point (cadr point))
		       (draw-point (caddr point)))
		     triangles))))

(define (RotateAfter ang x y z)
  (let ((m (f32vector)))
    (gl:GetFloatv gl:MODELVIEW_MATRIX m)
    (gl:LoadIdentity)
    (gl:Rotatef ang x y z)
    (gl:MultMatrixf m)))

(define (reshape w h)
  (gl:Viewport 0 0 w h)
  (gl:MatrixMode gl:PROJECTION)
  (gl:LoadIdentity)
  (let ((ar (/ w h)))
    (gl:Frustum (* -.1 ar) (* .1 ar) -.1 .1 .1 100))
  (gl:MatrixMode gl:MODELVIEW)
  (gl:LoadIdentity)
  (gl:Translatef 0.0 0.0 -10.0))

; render an alpha blended circle of the current gps location
(define (pos-draw pos radius color)
  (gl:BlendFunc gl:SRC_ALPHA gl:ONE_MINUS_SRC_ALPHA)
  (glLetEnable
   (gl:BLEND)
   (glLetMatrix
    (gl:Translatef (car pos) (cadr pos) (caddr pos))
    (glBegin gl:TRIANGLE_FAN
	     (gl:Color4f .5 .5 .5 1)
	     (gl:Vertex3f 0 0 0)
	     (gl:Color4f (first color) (second color) (third color) 0)
	     (do ((x 0 (+ x (/ pi 10))))
		 ((> x (+ (* 2 pi) .1)))
	       (gl:Vertex3f (* radius (cos x)) (* radius (sin x)) 0))))))

; called at the start of each frame to display
(define (gl-display)
  (gl:Clear gl:COLOR_BUFFER_BIT)
  
  (glLetMatrix
   (gl:Translatef 0 0 -1)
   (trianglegrid-draw)
   (pos-draw curpos horizdilution '(1 0 0)))

  (glut:SwapBuffers))

(define (none-false l)
  (fold (lambda (el acc) (and el acc)) #t l))

(define magmag 0)
(define (parse-mag-line line)
  (let ((vals (string-split line)))
    (if (and (>= (length vals) 3)
	     (equal? (car vals) "mag:"))
	(let ((magv (map string->number (take (cdr vals) 3))))
	  (if (none-false magv)
	      (set! magmag (magnitude magv)))))))

; convert lat lon and alt to x, y, z coordinates used for plotting
(define convert-position
  (let ((flat 100)
	(flon 100)
	(falt 100))
    (lambda (lat lon alt)
      (cond ((= flat 100)
	     (set! flat lat)
	     (set! flon lon)
	     (set! falt alt)))
      (let ((earthradius 6360000))
	(list
	 (* earthradius (sin (deg2rad (- lon flon))) (cos (deg2rad lat)))
	 (* earthradius (sin (deg2rad (- lat flat))))
	 (/ (- alt falt) 10) ; scale down alt a lot for now
	    )))))

(define (parse-gps-line line)
  (let ((vals (string-split line ",")))
    (cond ((null? vals) '())
	  ((and (> (length vals) 9)
		(equal? (car vals) "$GPGGA"))
	   (let ((time (string->number (list-ref vals 1)))
		 (north (string->number (list-ref vals 2)))
		 (ns (list-ref vals 3))
		 (east (string->number (list-ref vals 4)))
		 (ew (list-ref vals 5))
		 (fix (string->number (list-ref vals 6)))
		 (satcount (string->number (list-ref vals 7)))
		 (hd (string->number (list-ref vals 8)))
		 (alt (string->number (list-ref vals 9))))
	     (if (= fix 0)
		 (print "no gps fix, sats: " satcount)
		 (let ()
		   (define (convert-deg val)
		     (+ (floor (/ val 100))
			(/ (remainder val 100) 60)))
		   (if (< fix 1) ; hacked to turn on without DGPS
		       (print "warning, only normal GPS not DGPS fix, discarding")
		       (begin
			 (if (equal? "S" ns)
			     (set! north (- north)))
			 (if (equal? "W" ew)
			     (set! east (- east)))
			 (if verbose
			     (print north "N " east "E " alt "M " hd " HD "
				    (if (= 1 fix) " GPS fix " "DGPS fix ") satcount " sats"))
			 (set! curpos (append
				       (convert-position
					(convert-deg north)
					(convert-deg east)
					alt)
				       (list magmag)))
			 (set! horizdilution hd)
			 (reportdata curpos))))))))))
  
(define (make-line-reader)
  (let ((cur ""))
    (lambda (port)
      (if (char-ready? port)
	  (let loop ()
	    (let ((c (read-char port)))
	      (cond ((eof-object? c) #f)
		    ((equal? c #\newline)
		     (let ((ret (string-copy cur)))
		       (set! cur "")
		       ret))
		    (else (set! cur (string-append cur (string c))) (loop)))))
	  #f))))

(define idle
  (let ((mag-line (make-line-reader))
	(gps-line (make-line-reader)))
    (lambda ()
      (if mag-device
	  (let each-mag-line ()
	    (let ((line (mag-line mag-device)))
	      (cond (line (parse-mag-line line)
			  (each-mag-line))))))
      (if gps-device
	  (let each-gps-line ()
	    (let ((line (gps-line gps-device)))
	      (cond (line (parse-gps-line line)
			  (each-gps-line))))))
      (thread-sleep! .001)
      )))

(require-extension srfi-69)
(define attributes (make-hash-table))
(define (attr-set! name val)
  (hash-table-set! attributes name val))
(define (attr-get name)
  (hash-table-ref attributes name))
(attr-set! "linemode" #f)
(attr-set! "rotatespeed" 5)
(attr-set! "translatespeed" 1)

(define (glut-HasModifiers . modifiers)
  (let ((modifiers (apply bitwise-ior modifiers)))
    (= modifiers (bitwise-and modifiers (glut:GetModifiers)))))

(define (keyboard key x y)
  (case key
    ((#\esc #\q) (exit))
    ((#\w)
     (attr-set! "linemode" (not (attr-get "linemode")))
     (gl:PolygonMode gl:FRONT_AND_BACK
		     (if (attr-get "linemode")
			 gl:LINE gl:FILL)))
    ((#\f) (glut:FullScreen)))
  (glut:PostRedisplay))

(define (special key x y)
  (if (glut-HasModifiers glut:ACTIVE_SHIFT)
      (let ((rs (attr-get "rotatespeed")))
	(cond
	  ((= key glut:KEY_LEFT) (RotateAfter rs 0 1 0))
	  ((= key glut:KEY_RIGHT) (RotateAfter rs 0 -1 0))
	  ((= key glut:KEY_UP) (RotateAfter rs 1 0 0))
	  ((= key glut:KEY_DOWN) (RotateAfter rs -1 0 0))
	  ((= key glut:KEY_PAGE_UP) (RotateAfter rs 0 0 1))
	  ((= key glut:KEY_PAGE_DOWN) (RotateAfter rs 0 0 -1))))
      (let ((ts (attr-get "translatespeed")))
	(cond
	  ((= key glut:KEY_LEFT) (gl:Translatef ts 0 0))
	  ((= key glut:KEY_RIGHT) (gl:Translatef (- ts) 0 0))
	  ((= key glut:KEY_UP) (gl:Translatef 0 (- ts) 0))
	  ((= key glut:KEY_DOWN) (gl:Translatef 0 ts 0))
	  ((= key glut:KEY_PAGE_UP) (gl:Translatef 0 0 (- ts)))
	  ((= key glut:KEY_PAGE_DOWN) (gl:Translatef 0 0 ts))
	)))
  (glut:PostRedisplay))

(glut:InitDisplayMode (+ glut:DOUBLE glut:RGB glut:ALPHA))
(glut:CreateWindow "magplot")
(glut:ReshapeFunc reshape)
(glut:DisplayFunc gl-display)
(glut:IdleFunc idle)
(glut:KeyboardFunc keyboard)
(glut:SpecialFunc special)

(gl:ClearColor 0.0 0.0 0.0 0)

(use args)

(define (usage)
  (print "Usage: " (car (argv)) " [options...] [files...]")
  (newline)
  (print (args:usage opts))
  (print "Report bugs to geckosenator at gmail.")
  (exit 1))

(define output-file #f)
(define noshow #f)
(define verbose #f)
(define gps-device #f)
(define mag-device #f)

(define (reportdata pos)
  (let ((x (car pos))
	(y (cadr pos))
	(z (caddr pos))
	(mag (cadddr pos)))
    (if output-file
	(with-output-to-port output-file
	  (lambda () (print x " " y " " z " " mag))))
    (trianglegrid-addpoint x y z mag)
    (if (not noshow)
	(glut:PostRedisplay))))
  
(define (load-data)
  (let ((x (read)))
    (let ((y (read)))
      (let ((z (read)))
	(let ((mag (read)))
	  (cond ((not (eof-object? mag))
		 (reportdata (list x y z mag))
		 (load-data))))))))

(define (handle-open-file filename thunk)
  (thunk))
;  (with-exception-handler
;   (lambda _ (print "failed to open file: " filename) (exit))
;   thunk))

(define opts
 (list
  (args:make-option (h help) #:none "Display this text" (usage))
  (args:make-option (f file) (required: "FILENAME") "read input from a file"
		    (handle-open-file arg
				      (lambda ()
					(with-input-from-port
					    (if (equal? "-" arg)
						(current-input-port)
						(open-input-file arg))
					      load-data))))
  (args:make-option (o output) (required: "FILENAME") "write output to a file"
		    (if (equal? "-" arg)
			(set! output-file (current-output-port))
			(handle-open-file arg
					  (lambda ()
					    (set! output-file (open-output-file arg))))))
  (args:make-option (n noshow) #:node  "Don't display anything"
		    (set! noshow #t))
  (args:make-option (v verbose) #:node  "Don't display anything"
		    (set! verbose #t))
  (args:make-option (g gps)    (required: "DEVICE") "device for gps data"
		    ; set baud to 19200
		    (system (string-append "stty -F " arg " 19200"))
		    (handle-open-file arg
				      (lambda ()
					(set! gps-device (open-input-file arg)))))
  (args:make-option (m mag) (required: "DEVICE") "device for magnetometer data"
		    (handle-open-file arg
				      (lambda ()
					(set! mag-device (open-input-file arg)))))
  ))

(args:parse (command-line-arguments) opts)

(if noshow
    (let loop () (idle) (loop))
    (glut:MainLoop))
