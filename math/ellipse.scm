(load "matrix.scm")
(load "exp.scm")

; compute derivatives of one dimension less than vars for each column
; input gets additional column of 1's, and determinants are calculated
; for each submatrix
(define (dets vars)
  (define (skip-col n var)
    (let each ((x 0) (var var))
      (if (null? var)
          '()
          (let ((rest (each (+ x 1) (cdr var))))
            (if (= x n) rest
                (cons (car var) rest))))))

  (let loop ((sign 1) (n 0))
    (define (all-rows n)
      (map (lambda (var) (skip-col n var)) vars))
    (if (> n (length vars))
        '()
        (cons (* sign (determinant (all-rows n)))
              (loop (- sign) (+ n 1)))))
)

(define (fit-ellipse-2d vals)
  (dets
   (map (lambda (val)
          (let ((x (car val)) (y (cadr val)))
            (list (square x) (* x y) (square y) x y 1)))
        vals)))

(define ellipse-vars '(x^2 x*y y^2 x ()))

(define (display-ellipse vals)
  (show-exp
   ellipse-vars
   (fit-ellipse-2d vals))
  (display " = 0"))


;; lets solve!
; x' = u*x
; y' = v*x + w*y
; a*x^2 + b*x*y + c*y^2 = 1
; x'^2 + y'^2 = 1

; u^2*x^2 + (v*x + w*y)^2 = 1
; u^2*x^2 + v^2*x^2 + 2*v*w*x*y + w^2*y^2 = 1
; a = u^2 + v^2
; b = 2*v*w
; c = w^2

; w = sqrt(c)
; v = b/(2*w)
; u = sqrt(a-v^2)




Ax^2 + By^2 + Cx + Dy + Exy + F = 0
xt Aq x = 0

transpose(X) * Aq * X = 0;

X : matrix([1], [x], [y]);

Aq : matrix([F, C/2, D/2], [C/2, A, E/2], [D/2, E/2, B]);
A11 : matrix([A, E/2], [E/2, B]);

;; 3D case

X : matrix([1], [x], [y], [z]);

A*x^2 + B*y^2 + C*z^2 + D*x*y + E*y*z + F*z*x + G*x + H*y + J*z + K = 0;
Aq : matrix([K, G/2, H/2, J/2], [G/2, A, D/2, F/2], [H/2, D/2, B, E/2], [J/2, F/2, E/2, C]);
A11 : submatrix(1, Aq, 1);

-- take bottom 3 rows in Aq, solve for x y and z to get centers
solve([G/2 + A*x + D/2*y + F/2*z = 0, H/2 + D/2*x + B*y + E/2*z = 0, J/2 + F/2*x + E/2*y + C*z=0], [x, y, z]);

q = (2*B*F^2-2*D*E*F+A*(2*E^2-8*B*C)+2*C*D^2)
x=-(D*(2*C*H-E*J)+F*(2*B*J-E*H)+(E^2-4*B*C)*G) / q
y= (A*(4*C*H-2*E*J)+D*F*J-F^2*H+(E*F-2*C*D)*G) / q
z=-(A*(2*E*H-4*B*J)+D^2*J-D*F*H+(2*B*F-D*E)*G) / q


(define (optimize exp)
        (pp-expr/C (optimize-expr (string-infix->prefix exp))'FOO 'double))

(optimize "(2*B*F^2-2*D*E*F+A*(2*E^2-8*B*C)+2*C*D^2)")

q=((2 * B * F * F) - ((2 * ((C * D * D) + (D * E * F))) + (A * ((2 * E * E) - (8 * B * C)))));
x=-((D*((2*C*H) - (E*J))) + (F*((2*B*J) - (E*H))) + (((E*E) - (4*B*C))*G)) / q;
y= ((A*((4*C*H) - (2*E*J))) + ((D*F*J) - ((H*F*F) + (((E*F) - (2*C*D))*G)))) / q;
z=-((A*((2*E*H) - (4*B*J))) + ((J*D*D) - ((D*F*H) + (((2*B*F) - (D*E))*G)))) / q;

(define (string-reverse str)
       

(load "infix2prefix.scm")

--Aq : matrix([K, G, H, J], [0, A, D, F], [0, 0, B, E], [0, 0, 0, C]);

transpose(X) . Aq . X;

