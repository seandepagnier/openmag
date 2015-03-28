(load "matrix.scm")
(load "linalg.scm")

(define (average vals)
  (/ (fold + 0 vals) (length vals)))

(define (average-point points)
  (map average (transpose points)))

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

; for any dimensional plane, eg: a*x + b*y + c*z ... = d
(define (fit-plane points)
  (let ((avgpoint (average-point points)))
    (let ((moved-points
           (map (lambda (point)
                  (map - point avgpoint))
                points)))
      (let ((K (list (make-list (length points) 1))))
        (let ((R (normalize (diff moved-points K))))
          (append R (list (dot avgpoint R))))))))

(define points
  '((-0.0437 -0.0408 0.9982)
    (-0.0087 0.9716 0.2365)
    (-0.0008 -1.0000 -0.0057)
    (0.0132 -0.1499 -0.9886)))

(define align-points
  '((-0.01 -0.23 -0.97)
    (0.17 0.32 -1.32)
    (-0.02 0.24 -1.31)
    (-0.24 -0.10 -1.35)
    (0.27 -0.27 -1.42)))

(define more-points
  '((-0.24 0.29 0.93)
    (-0.02 0.38 0.93)
    (-0.18 0.33 0.93)
    (0.11 0.36 0.93)
    (-.44 .06 .91)
    (0.02 -0.41 0.91)
    (-0.06 0.37 0.93)
    (.43 -.08 .9)))

