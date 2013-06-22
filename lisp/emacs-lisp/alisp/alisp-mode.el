
(defgroup alisp nil
  "Lisp support, including Emacs Lisp."
  :link '(custom-group-link :tag "Font Lock Faces group" font-lock-faces)
  :group 'languages
  :group 'development)

(defcustom alisp-mode-hook nil
  "Hook run when entering alisp mode."
  :type 'hook
  :group 'alisp)

(defun alisp-hello ()
  "Alisp mode's build in func"
  (interactive)
  (message "Hello world."))

(defconst alisp-font-lock-keywords
  (append lisp-font-lock-keywords-2
          (eval-when-compile 
            `((,(concat "(" (regexp-opt 
                             '("Acall" "Aerror" "Ahandle" "Aresult" "Asnderr" "Asyserr")
                             t)
                        "\\>") . 1)))))

(defvar alisp-mode-map
  (let ((map (make-sparse-keymap)))
    (define-key map "\e\C-i" 'alisp-hello)
    map)
  "Keymap for alsa lisp mode.")

(defvar alisp-mode-syntax-table 
  (let ((table (make-syntax-table))
        (i 0))
        (while (< i ?0)
      (modify-syntax-entry i "_   " table)
      (setq i (1+ i)))
    (setq i (1+ ?9))
    (while (< i ?A)
      (modify-syntax-entry i "_   " table)
      (setq i (1+ i)))
    (setq i (1+ ?Z))
    (while (< i ?a)
      (modify-syntax-entry i "_   " table)
      (setq i (1+ i)))
    (setq i (1+ ?z))
    (while (< i 128)
      (modify-syntax-entry i "_   " table)
      (setq i (1+ i)))
    (modify-syntax-entry ?\s "    " table)
    ;; Non-break space acts as whitespace.
    (modify-syntax-entry ?\x8a0 "    " table)
    (modify-syntax-entry ?\t "    " table)
    (modify-syntax-entry ?\f "    " table)
    (modify-syntax-entry ?\n ">   " table)
    ;; This is probably obsolete since nowadays such features use overlays.
    ;; ;; Give CR the same syntax as newline, for selective-display.
    ;; (modify-syntax-entry ?\^m ">   " table)
    (modify-syntax-entry ?\; "<   " table)
    (modify-syntax-entry ?` "'   " table)
    (modify-syntax-entry ?' "'   " table)
    (modify-syntax-entry ?, "'   " table)
    (modify-syntax-entry ?@ "'   " table)
    ;; Used to be singlequote; changed for flonums.
    (modify-syntax-entry ?. "_   " table)
    (modify-syntax-entry ?# "'   " table)
    (modify-syntax-entry ?\" "\"    " table)
    (modify-syntax-entry ?\\ "\\   " table)
    (modify-syntax-entry ?\( "()  " table)
    (modify-syntax-entry ?\) ")(  " table)
    (modify-syntax-entry ?\[ "(]  " table)
    (modify-syntax-entry ?\] ")[  " table)
    table)
  "Syntax table used in alsa lisp mode.")

(defvar alisp-mode-abbrev-table nil)
(define-abbrev-table 'alisp-mode-abbrev-table ()
  "Abbrev table for Alisp mode")

(defun alisp-indent-line (&optional whole-exp)
  "Indent alisp code line, same as lisp code"
  (interactive "P")
  )

;; *font-lock-defaults
;; *indent
;; *comment
;; *key-map
;; *syntax-table
;; *abbreve-table
(define-derived-mode alisp-mode prog-mode "Alsa Lisp"
  "Major mode for edit Alsa lisp code"
  :group 'alisp
  (setq font-lock-defaults
        '((lisp-font-lock-keywords-1 lisp-font-lock-keywords-2
                                     alisp-font-lock-keywords)
          nil nil
          (("+-*/.<>=!?$%_&~^:@" . "w")) nil))
  (make-local-variable 'indent-line-function)
  (setq indent-line-function 'alisp-indent-line)
  (make-local-variable 'comment-start)
  (setq comment-start ";")
  (make-local-variable 'comment-start-skip)
  ;; Look within the line for a ; following an even number of backslashes
  ;; after either a non-backslash or the line beginning.
  (setq comment-start-skip "\\(\\(^\\|[^\\\\\n]\\)\\(\\\\\\\\\\)*\\);+ *")
  (make-local-variable 'font-lock-comment-start-skip)
  ;; Font lock mode uses this only when it KNOWS a comment is starting.
  (setq font-lock-comment-start-skip ";+ *")
  (make-local-variable 'comment-add)
  (setq comment-add 1)			;default to `;;' in comment-region
  (make-local-variable 'comment-column)
  (setq comment-column 40)
  ;; Don't get confused by `;' in doc strings when paragraph-filling.
  (set (make-local-variable 'comment-use-global-state) t)
)

(add-to-list 'auto-mode-alist '("\\.alisp\\'" . alisp-mode))

(provide 'alisp-mode)
