@Echo Off
REM Скрипт для автоматического обновления репозиториев в папках, указанных в конфиге // by Robilkot
REM ! Требует настроенной upstream branch в репозитории !

@FOR /F %%x IN ('findstr /B /V /C:# robilkot_autopull_repos.txt') do (
echo --- Updating repo at %%x ---
pushd "%%x"
call git pull
popd
)
echo --- All repos updated! ---