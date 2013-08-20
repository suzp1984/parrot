(defun get-symbols (lst)
  (when lst
    (if (atom lst) (list lst)
      (append (get-symbols (car lst))
              (get-symbols (cdr lst))))))

(defun reverse-list (a)
  (let ((b (cdr a))
        )
    (if b
        (append (get-symbols (reverse-list b)) (get-symbols (car a)))
      a)
    ))

(reverse-list  '(1 2 3 4 5 6 7 8))
(reverse-list '(1))
(reverse '(1 2 3))

(get-symbols '(1 2 3 (5 6)))
