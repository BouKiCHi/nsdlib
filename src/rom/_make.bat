del buildlog.txt
del errlog.txt
del comlog.txt
make -k >buildlog.txt 2>&1
if %errorlevel% equ 0	goto end
start errlog.txt
:end
