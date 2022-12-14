# Autopull

Программа предназначена для автоматического обновления репозиториев, указанных в конфиге.

<hr>

Для работы требуется добавить ярлык на приложение в автозагрузку (либо создать задачу при запуске ПК в планировщике). Также можно запускать вручную для обновления множества репозиториев одним кликом.

В качестве входного параметра программе можно дать текстовый файл-конфиг. Если программа запущена без аргументов, будет открыт файл со стандартным названием robilkot_autopull_repos.txt в рабочей папке программы.

Формат конфига имеет вид:
[Путь к репозиторию на диске] [Имя remote] [Имя ветки]

*Указание remote и ветки опционально. При настроенной upstream branch обновление произойдет и без их указания.*

<hr>

После неудачной попытки обновления будет выполнено еще две попытки с интервалом в 5 и 10 секунд (если ошибка не критическая). Это позволяет избежать проблем с задержкой подключения к интернету при запуске компьютера.

При возникновении ошибки программа пишет причину. Если же возникла unknown error - рекомендуется обновить репозиторий вручную.

В конце выполнения программы выводится сообщение с количеством обновленных репозиториев из общего числа указанных.
Если обновление всех репозиториев прошло успешно, программа закрывается самостоятельно. Иначе требуется нажать любую клавишу для выхода.

<hr>

![image](https://user-images.githubusercontent.com/82116328/208173028-8b9be6bb-834e-4fa3-8501-76ab26f50169.png)
