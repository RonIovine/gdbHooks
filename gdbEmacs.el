;; ===================================================================
;; Add color to the current GUD line (obrigado google)
;; ===================================================================

(defvar gud-overlay
  (let* ((ov (make-overlay (point-min) (point-min))))
    (overlay-put ov 'face 'secondary-selection)
    ov)
  "Overlay variable for GUD highlighting.")

(defadvice gud-display-line (after my-gud-highlight act)
           "Highlight current line."
           (let* ((ov gud-overlay)
                  (bf (gud-find-file true-file)))
             (save-excursion
                 (set-buffer bf)
                   ;; the following statement highlights the whole line for the window width
                   (move-overlay ov (line-beginning-position) (line-beginning-position 2)
                   ;; the following statement just highlights to the EOL character
                   ;;(move-overlay ov (line-beginning-position) (line-end-position)
                                   (current-buffer)))))

(defun gud-kill-buffer ()
  (if (eq major-mode 'gud-mode)
    (delete-overlay gud-overlay)))

(add-hook 'kill-buffer-hook 'gud-kill-buffer)

(custom-set-variables
 '(scroll-bar-mode (quote right)))

;; =================================
;; GDB configuration
;; =================================

(require 'linum)
(global-linum-mode 1)

(require 'gud)
;;(require 'gdb-ui)
(setq gdb-many-windows t)
(add-hook
 'gdb-mode-hook
 '(lambda ()
    (gud-tooltip-mode t)
    (gud-def gud-break-main "break main" nil "Set breakpoint at main.")
 ))
(setq gud-tooltip-echo-area nil);;(setq gdb-display-threads-buffer t)
(eval-after-load "gud"
  '(progn
     (define-key gud-mode-map (kbd "<up>") 'comint-previous-input)
     (define-key gud-mode-map (kbd "<down>") 'comint-next-input)))
