onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib fifo_maxtrix_opt

do {wave.do}

view wave
view structure
view signals

do {fifo_maxtrix.udo}

run -all

quit -force
