(load "matrix.scm")
(load "linalg.scm")

;(require-extension (srfi 1))

(define (square x) (* x x))
(define (cube x) (* x x x))

;; compute jacobian and residules for a circle
;; (x0 - xi)^2 + (y0 - yi)^2 = r^2
(define (circle-proc values point)
  (let ((x0 (car values))
        (y0 (cadr values))
        (r0 (caddr values))
        (xi (car point))
        (yi (cadr point)))
    (let ((dx (- x0 xi))
          (dy (- y0 yi)))
      (list
       (let ((ri (sqrt (+ (square dx) (square dy)))))
         (list (/ dx ri)
               (/ dy ri)
               -1))
       (- r0 (sqrt (+ (square dx)
                      (square dy))))))))

;; ellipse   a0*(x0-xi)^2 + b0*(y0-yi)^2 = 1
(define (ellipse-proc values point)
  (let ((a0 (car values))
        (b0 (cadr values))
        (x0 (caddr values))
        (y0 (cadddr values))
        (xi (car point))
        (yi (cadr point)))
    (let ((dx (- x0 xi))
          (dy (- y0 yi)))
      (list
       (list (square dx)
             (square dy)
             (* 2 a0 dx)
             (* 2 b0 dy))
       (- 1 (+ (* a0 (square dx)) (* b0 (square dy))))))))

;; a*x^2 + b*x*y + c*y^2 = 1
(define (test-proc values point)
  (let ((a0 (car values))
        (b0 (cadr values))
        (c0 (caddr values))
        (xi (car point))
        (yi (cadr point)))
    (list
     (list (* xi xi)
           (* xi yi)
           (* yi yi))
     (- 1 (+ (* a0 xi xi) (* b0 xi yi) (* c0 yi yi))))))

; x = p*xp + x0
; y = q*xp + r*yp + y0
;
; a*xp^2 + b*xp*yp + c*yp^2 + d*xp + e*yp = 1
;
; r^2*yp^2 + 2*r*y0*yp + 2*q*r*xp*yp + y0^2 + 2*q*xp*y0
;   + q^2*xp^2 + p^2*xp^2 + 2*p*x0*xp + x0^2 = 1
; ((q^2 + p^2)*xp^2 + 2*q*r*xp*yp + r^2*yp^2
;   (2*q*y0 + 2*p*x0)*xp + 2*r*y0*yp) / (1 - y0^2 + x0^2) = 1
;
; t = (1 - y0^2 + x0^2)
; a = (q^2 + p^2) / t
; b = 2*q*r / t
; c = r^2 / t
; d = (2*q*y0 + 2*p*x0) / t
; e = 2*r*y0 / t

; solve([a = (q^2 + p^2), b = 2*q*r, c = r^2 / (1 - y0\^2 + x0^2), d = (2*q*y0 + 2*p*x0), e = 2*r*y0], [p, q, r, x0, y0]);

(define (rotated-ellipse-proc values point)
  (let ((a0 (list-ref values 0))
        (b0 (list-ref values 1))
        (c0 (list-ref values 2))
        (d0 (list-ref values 3))
        (e0 (list-ref values 4))
        (xi (car point))
        (yi (cadr point)))
    (list
     (list (* xi xi)
           (* xi yi)
           (* yi yi)
           xi
           yi)
     (- 1 (* a0 xi xi) (* b0 xi yi) (* c0 yi yi) (* d0 xi) (* e0 yi)))))

; a*x^2 + b*y^2 + c*z^2 + d*x*y + e*y*z + f*z*x + g*x + h*y + i*z = 1
(define (rotated-ellipse-3d-proc values point)
  (let ((a0 (list-ref values 0)) (b0 (list-ref values 1)) (c0 (list-ref values 2))
        (d0 (list-ref values 3)) (e0 (list-ref values 4)) (f0 (list-ref values 5))
        (g0 (list-ref values 6)) (h0 (list-ref values 7)) (i0 (list-ref values 8))
        (xi (car point)) (yi (cadr point)) (zi (caddr point)))
    (list
     (list (* xi xi) (* yi yi) (* zi zi)
           (* xi yi) (* yi zi) (* zi xi)
           xi yi zi)
     (- 1
        (* a0 (cube xi)) (* b0 (cube yi)) (* c0 (cube zi))
        (* d0 xi yi) (* e0 yi zi) (* f0 zi xi)
        (* g0 xi) (* h0 yi) (* i0 zi)))))

;(approx 10 rotated-ellipse-3d-proc '(1 1 1 0 0 0 0 0 0) p)
(define p '((1 0 0) (-1 0 0) (0 1 0) (0 -1 0) (0 0 1) (0 0 -1)
             (1/10 7/10 7/10) (4/10 2/10 6/10) (3/10 4/10 5/10)
             (4/13 3/24 2/4) (4/3 3/4 4/4)))

; given a proc that computes one row of a jacobian and residual,
; compute both matrixes

(define (exec-proc proc values points)
  (list
   (map (lambda (point) ; jacobian
          (car (proc values point)))
        points)
   (list ; residules
    (map (lambda (point)
           (cadr (proc values point)))
         points))))

; given a jacobian and residuals matrix, compute the update
(define (diff J K)
  (map car (mult-matrix
            (mult-matrix
             (let ((inv (invert (mult-matrix (transpose J) J))))
               (if (eq? inv 'none)
                   (error "cannot take inverse")
                   inv))
             (transpose J))
            (transpose K))))

; compute a least squares fit given iterations, initial esimate,
; data, and a proc to compute the jacobian and residuals
; eg: (approx 10 ellipse-proc '(1 1 0 0) '((0 2) (2 0) (-2 0) (0 -2)))
(define (approx iters proc values points)
  (if (= iters 0) values
      (let ((p (exec-proc proc values points)))
        (approx (- iters 1)
                proc
                (map + values (diff (car p) (cadr p)))
                points))))

(define magpoints '((9632.98 9451.89)
                    (-1537.71 -1405.24)
                    (16213.13 -3837.64)
                    (5173.36 9744.61)
                    (757.80 -5557.94)
                    (9934.17 -8914.94)
                    (13044.38 7577.20)
                    (15532.46 4655.12)
                    (-965.34 5104.87)
                    (5420.13 -8556.02)))
