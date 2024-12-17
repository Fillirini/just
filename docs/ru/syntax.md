# Синтаксис языка Just
Анализ вашего исходного кода проходит на нескольких уровнях. Первый - этап токенизации.
Во время токенизации (еще называют лексическим анализом) текст разбивается на `слова` на основе определенной последовательности `символов`.
Второй этап - синтаксический анализ. Парсер работает на более высоком уровне, анализируя `слова`, а не отдельные `символы`. Он генерирует **Абстрактное Синтаксическое Дерево (AST)**, удобное представление кода для семантического анализа, простейшей кодогенерации и интерпретации.
Третий этап - семантический анализ. Анализирует код для поиска смысловых ошибок, таких как, вызов несуществующей функции или передача в функцию `sin` строки вместо числа.

# Токенизация
Вот список разновидностей слов и правил, по которым они распознаются:

## Идентификатор и ключевое слово 
Начинаются с буквы или '&lowbar;'.
Последующие символы: буквы, десятичные цифры или '&lowbar;'.

Пример: 'main', 'var', 'entry', 'speedOfPlayer_123', '_коэфРазложения'.

Если идентификатор с таким строковым значением содержится в `хранилище ключевых слов*`, то тип этого токена меняется с `Ident` на `KeyWord` (ключевое слово).

## Числа
Любое число должно начинаться с десятичной цифры. 
Последующие символы: десятичные цифры и '&lowbar;' (для красоты и удобочитаемости больших чисел).

Самое интересное начинается тогда, когда токенизатор встречает символ точки ('.'). В языке Just у нее есть несколько значений:
1. Оператор обращения к членам структуры.
2. Десятичный разделить в дробных числах. 

Анализаторы старых языков опускали возможность обращения к числовым литералам (хотя, справедливости ради, в некоторых из них не было методов, как в Си). Однако, современные стандарты определяют значение `точки` в каждом конкретном случае и позволяют вам вызвать метод прямо у литерала, так что в Just эта возможность тоже реализована.
А теперь подробнее о механизме детектирования:

0) Начало токенизации числа.\
Если первый символ `'0'`, а второй `x`, `o` или `b` (причем регистр буквы не важен), то переходим к **пункту 4**.\
Иначе, считываем десятичные цифры.\
Если встретили `точку`, то переходим к **пункту 1**.\
Если встретили букву, то переходим к **пункту 3**.\
Если встретили какой-то иной символ, то генерируем токен типа `Int` - простое целое число, записанное десятичными цифрами, конкретный тип не указан (например: `i32`, `u8`, `f64`. подробнее в **пункте 3**).
1) Токенизатор впервые встречает `точку ('.')` (это не смайлик😀) в целом числе.\
Если следующий символ десятичная цифра - переходим к **пункту 2**.\
В противном случае генерируем токен `IntWithDot`, равный `123.` для строки `123.pow(2)` или `123. pow(2)`, пробелы между словами не имеют значения. После нее токенизатор начнет анализ как ни в чем не бывало.\
Далее, на более высоком уровне, а именно на стадии синтаксического анализа (придающего смысл наших словам, токенам) принимается решение:
    * Если после токена `IntWithDot` идет идентификатор, то последовательность будет трактоваться как обращение к члену целого числа (подходит строка `123.pow(2)`, преобразующаяся в токены `IntWithDot("123.")`, `Ident("pow")`, ...). 
   * В противной случае `IdentWithDot` будет считаться за дробное число (`var x = 123.;` => ..., `IntWithDot("123.")`, ...)
2) После `точки` идет цифра. Значит это дробное число. Последующие символы должны быть десятичными цифрами.\
Если токенизатор снова встречает символ `точки` то она однозначна принимается за `оператор обращения к члену структуры`. То есть, если после записи `123.456.` не идет идентификатор, парсер сгенерирует **синтаксическую ошибку**, что-то в роде `ожидается идентификатор`.\
Если токенизатор встречает буквы, то переходим к **пункту 3**.\
Если встретили какой-либо другой символ, то генерируем токен `Float` - дробное число без определенного типа (подробнее о типах в **пункте 3**)
3) Сразу после числа идут буквы. Токенизатор принимает эту букву за начало так называемого `постфикса`. Постфикс - частичка в конце литерала, уведомляющая компилятор (именно компилятор, интерпретатору все равно на эту аннотацию, он всегда создает большое 64-битное число, живите с этим) о типе и размере числа. Так, вы можете обозначить `123` (заметьте, в этой строке нет `точки`) как дробное число, дописав к нему, например, `f64`.\
Так вот. При встрече букв генерируется токен типа `IntWithPostfix` или `FloatWithPostfix`. Следующия итерация токенизирует постфикс и генеририрует `Ident`. То есть, `123i32` = `IntWithPostfix("123")`, `Ident("i32")`. Не бойтесь, парсер обратит внимание на пометку `WithPostfix` и поймет, что следующий токен должен быть типа `Ident` и он на самом деле это постфикс.
4) Первый символ `'0'`, а второй `x`, `o` или `b`. Значит это **целые** (именно целые! дробные числа записываются только десятичными цифрами) числа, записанные в других системах счисления (далее СС).\
Если `x`, то это число записано в 16-ичной СС. Последующие символы десятичные цифры или буквы `A-F` (регистр не важен).\
Если `o`, то это число записано в 8-ичной СС. Последующие символы `'0'-'7'`.\
Если `b`, то это число записано в 2-ичной СС. Последующие символы `'0'` или `'1'`.\
Если встретили `точку`, то она трактуется как `оператор обращения к члену структуры`, ведь дробные числа записываются только в 10-ичной СС.\
16-ичные числа содержат часть букв в записи. Из-за этого часть кастомных префиксов, начинающихся с `A-F` не будут работать. Однако, стандартные префиксы `i8-i64` и `u8-u64` распознаются без проблем. 
5. До этого мы рассмотрели числа, начинающиеся с десятичных цифр. Однако, такая запись не всегда удобна. Поэтому вы можете записывать дробные числа без целой части, начиная с символа `точки`. Например, `.64`. Так вот, если число начинается с точки, то переходим сразу к **пункту 2**.

Кстати, для красивой и удобочитаемой записи чисел вы можете добавлять в них символ `'_'`. Причем, их число не ограничено, можете записать хоть `1______1_`. Однако, помните, что число **НЕ МОЖЕТ** начинаться с `'_'` (это начало идентификаторов) и после `точки` в качестве десятичного разделителя дробного числа **НЕ МОЖЕТ** идти `'_'`, иначе он будет принят за начало идентификатора. То есть, `123._456` => `IntWithDot("123.")`, `Ident("_456")`. Данная ошибка обнаружится только при семантическом анализе (а именно на этапе разрешения ссылок). Анализатор сгенерирует что-то типа `структура Int не содержит метода "_456"`. 