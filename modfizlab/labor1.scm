#! /usr/local/bin/guile -s
!#

(use-modules (ice-9 rdelim))
(set-port-encoding! (current-output-port) "UTF-8")

(define *racsallando* 1000)

(define *cadmium*
  '("Cd.txt" (467.8149 479.9912 508.5822 643.8470 646.494)))

(define *H-D* "H-D.txt")
(define *Na* "Na.txt")
(define *K* "K.txt")
(define *Ne* "Ne.txt")

(define *cd-header*
  '("szín"
    "fok"
    "perc"
    "radián"
    "$\\lambda_{\\textrm{mért}} [nm]$"
    "$\\lambda_{\\textrm{irodalmi}} [nm]$"
    "hiba [nm]"))

(define *generic-header*
  '("szín"
    "fok"
    "perc"
    "radián"
    "$\\lambda_{\\textrm{mért}} [nm]$"
    "$\\lambda_{\\textrm{korrigált}} [nm]$"
    "hiba [nm]"))


;; ezzel váltunk radiánba
(define (degree->radian num)
  (* 2 3.141592654 (/ num 360)))

(define (load-data-file name)
  (map (lambda (element)
	 ;; az indexet lehagyja mert nincs értelme
	 (list (cadddr element)
	       (string->number (cadr element))
	       (string->number (caddr element))))
       (call-with-input-file name
	 (lambda (data-file)
	   ;; Rekurzív függvény. Kupacba ( az adatstruktúrába ) pakolja az adatokat,
	   ;; majd a fájl végére érve az egész listát visszaforgatja, hogy az adatok
	   ;; ne fordított sorrendben legyenek benne
	   ;; Azt, hogy a sorokon végigiteráljon, a rekurzió biztosítja
	   (let get-line ((lines '()))
	     (let ((line (read-line data-file)))
	       (if (eof-object? line)
		   ;; Itt a visszaforgatás
		   (reverse lines)
		   ;; Egyébként hozzáfűz
		   (get-line (cons
			      (string-split line #\space)
			      lines)))))))))



;; fokba alakít, majd radiánba
(define (data->rads data)
  (degree->radian (+ (car data) (/ (cadr data) 60))))


;; hullámhosszat számít
(define (rads->wl data)
  (* *racsallando* (sin data)))


(define (prepare data)           (data->rads (cdr     data)))

(define (generate-table data caption header ref-name)
  (let ((skeleton (open-output-string)))
    (begin
      (display "\\begin{table}[H]" skeleton)
      (newline skeleton)
      (display "\\centering" skeleton)
      (newline skeleton)
      (display "\\begin{tabular}{" skeleton)
      
      (for-each (lambda (x)
		  (display "|l" skeleton))
		(car data))
      
      (display "|}" skeleton)
      (newline skeleton)
      (display "\\hline " skeleton)
      (display (car header) skeleton)
      
      (for-each (lambda (x)
		  (display " & " skeleton)
		  (display x skeleton))
		(cdr header))
      
      (display " \\\\ \\hline \\hline" skeleton)
      (newline skeleton)

      (for-each (lambda (measurement)
		  (display (car measurement) skeleton)
		  (for-each (lambda (x)
			      (display " & " skeleton)
			      (format skeleton "~3,4f" x))
			    (cdr measurement))
		  (display " \\\\ \\hline" skeleton)
		  (newline skeleton))
		data)
      (display "\\end{tabular}" skeleton)
      (newline skeleton)
      
      (display "\\caption{" skeleton)
      (display caption skeleton)
      (display "}" skeleton)
      (newline skeleton)
      
      (display "\\label{tab:" skeleton)
      (display ref-name skeleton)
      (display "}" skeleton)
      (newline skeleton)
      
      (display "\\end{table}" skeleton))
    (get-output-string skeleton)))

(define (generate-gnuplot data)
  (let ((plotdata (open-output-string)))
    (for-each (lambda (x)
		(display (list-ref x 4) plotdata)
		(display " " plotdata)
		(display (- (list-ref x 4)(list-ref x 5)) plotdata)
		(newline plotdata))
	      data)
    (get-output-string plotdata)))

(define (diffs data) (map - (cdr data) (reverse (cdr (reverse data)))))

(define (debb wat)
  (display wat)
  wat)

(define (generate-error-function cd-data)
  (let* ((points (map cons
		      (map /
			   (diffs (map (lambda (x)
					 (list-ref x 6))
				       cd-data))
			   (diffs (map (lambda (x)
					 (list-ref x 4))
				       cd-data)))
		      (map (lambda (point)
			     (list (list-ref point 4)
				   (list-ref point 6)))
			   (cdr cd-data)))))
    ;; ( hibaváltozás hely hibaérték )
    (lambda (x)
      (let helper ((x x)
		   (plist points))
	(if (or (null? (cdr plist)) (< x (cadar plist)))
	    (+ (caddar plist) (* (- x (cadar plist)) (caar plist)) )
	    (helper x (cdr plist)))))))

(define (generic-load file)
  (let* ((raw (load-data-file file))
	 (initial (map prepare
		       (sort raw (lambda (y x)
				   (< (prepare y)
				      (prepare x)))))))
    (map list
	 (map car raw)
	 (map cadr raw)
	 (map caddr raw)
	 initial
	 (map rads->wl initial)
	 (map + (map errf (map rads->wl initial)) (map rads->wl initial))
	 (map errf (map rads->wl initial)))))


(define cd-data
  (let* ((raw (load-data-file (car *cadmium*)))
	 (initial (map prepare
		       (sort raw (lambda (y x)
				   (< (prepare y)
				      (prepare x)))))))
    (map list
	 (map car raw)
	 (map cadr raw)
	 (map caddr raw)
	 initial
	 (map rads->wl initial)
	 (cadr *cadmium*)
	 (map - (map rads->wl initial) (cadr *cadmium*)))))


(define cdtex (open-output-file "Cd.tex"))
(display (generate-table cd-data "Kadmium" *cd-header* "cd") cdtex)
(close-output-port cdtex)

(define gnuplot (open-output-file "plot.txt"))
(display (generate-gnuplot cd-data) gnuplot)
(close-output-port gnuplot)

(define errf (generate-error-function cd-data))
;; most már tudunk hibát számolni

(define H-D-data (generic-load *H-D*))

;;(display H-D-data)

(define H-D-tex (open-output-file "H-D.tex"))
(display (generate-table H-D-data "Hidrogén-deutérium" *generic-header* "h-d") H-D-tex)
(close-output-port H-D-tex)

(define K-data (generic-load "K.txt"))

(define K-tex (open-output-file "K.tex"))
(display (generate-table K-data "Kálium" *generic-header* "k") K-tex)
(close-output-port K-tex)

(define Na-data (generic-load "Na.txt"))

(define Na-tex (open-output-file "Na.tex"))
(display (generate-table Na-data "Nátrium" *generic-header* "na") Na-tex)
(close-output-port Na-tex)

(define Ne-data (generic-load "Ne.txt"))

(define Ne-tex (open-output-file "Ne.tex"))
(display (generate-table Ne-data "Neon" *generic-header* "ne") Ne-tex)
(close-output-port Ne-tex)

