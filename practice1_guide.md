# Practice1
## Задача
Проверить принадлежит суффикс заданного формата в языке
## Алгоритм
1. построим минимальный дка по регулярному выражению.
2. "развернем" ребра.
3. для каждой терминальной вершины проверим принадлежность префикса заданного формата.

## Сборка проекта
1. выполнить build.sh
2. выполнить build_coverage_report.sh
3. html файл с покрытием лежит в директории report с названием index.html
4. бинарные файлы: test и main, лежат в bin