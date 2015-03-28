(load "linalg.scm")

(define (randf)
  (/ (random 1000000) 1000000))

(define (get-xyz r s t u v w p q)
  (list
   (* r (cos p) (cos q))
   (* s (sin (+ u p)) (cos q))
   (* t (+ (* (cos (- v p)) (sin w) (cos q)) (* (cos w) (sin q))))))

(define (get-xyz-rand r s t u v w count)
  (if (= count 0) '()
      (cons (get-xyz r s t u v w (* 2 3.14159 (- (randf) .5)) (* 3.14159 (- (randf) .5)))
            (get-xyz-rand r s t u v w (- count 1)))))

; x = r*cos(p)*cos(q)
; y = s*sin(u+p)*cos(q)
; z = t*sin(v+p)*sin(w+q)
; x' = x/r
; y' = (y/s - sin(u)*x') / cos(u)
; z' = (z/t-sin(w)*[sin(v)*x' + cos(v)*y']) / cos(w)
(define (apply-cal r s t u v w x y z) 
  (let* ((xp (/ x r))
         (yp (/ (- (/ y s) (* (sin u) xp)) (cos u)))
         (zp (/ (- (/ z t) (* (+ (* (cos v) xp) (* (sin v) yp)) (sin w))) (cos w))))
    (list xp yp zp)))

(define (get-pq vec)
  (let ((nvec (normalize vec)))
    (let ((q (asin (caddr nvec))))
      (let ((p (asin (/ (cadr nvec) (cos q)))))
        (list p q)))))

;; NOTE: not useful.. results in imaginary answers when xyz doesn't lie on surface
(define (alt-pq r s t u v w x y z)
  (let ((p (atan (/ (* (/ (cos u)) (- (* r y) (* s x (sin u)))) (* s x)))))
    (let ((q (acos (/ x (* r (cos p))))))
      (list p q))))

