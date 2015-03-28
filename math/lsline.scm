(load "matrix.scm")

(define (average vals)
  (/ (fold + 0 vals) (length vals)))

(define (average-point points)
  (map average (transpose points)))

(define (lsline x)
  (if (< (length x) 2)
      (error "At least 2 data points required"))
  (let ((x0 (average-point x)))    ; calculate centroid
    (let ((A (map (lambda (y)           ; form matrix A of translated points
                    (map (lambda (a b)
                           (- a b))
                         y x0))
                  x)))
      A)))

%
% calculate the SVD of A
  [U, S, V] = svd(A, 0);
%
% find the larger singular value in S and extract from V the
% corresponding right singular vector
  [s, i] = max(diag(S));
  a = V(:, i);

% 
% calculate residual distances, if required  
  if nargout > 2 
    n = [-a(2) a(1)]'; 
    d = A * n; 
    normd = norm(d); 
  end % if nargout 
% ---------------------------------------------------------------------
% End of LS2DLINE.M
