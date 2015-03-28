(define (magnitude vec)
  (sqrt (apply + (map square vec))))

(define (normalize vec)
  (let ((mag (magnitude vec)))
    (map (lambda (x) (/ x mag)) vec)))

(define (dot . vecs)
  (apply + (apply map * vecs)))
