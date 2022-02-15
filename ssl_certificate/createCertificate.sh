#!/bin/bash

# -------------------------------------------------------- #
#Создание собственного самоподписанного доверенного сертификата.
openssl req -new -newkey rsa:2048 -nodes -keyout rootca.key -x509 -days 1000 \
          -subj /C=RU/ST=Msk/L=Msk/O=tellec\ Inc/OU=Sale/CN=bla/emailAddress=usr@dom.ru \
          -out rootca.crt
          
          
# req Запрос на создание нового сертификата.
# -new Создание запроса на сертификат (Certificate Signing Request - далее CSR).
# -newkey rsa:1023   Автоматически будет создан новый закрытый RSA ключ длиной 1024
#    бита. Длину ключа можете настроить по своему усмотрению.
# -nodes Не шифровать закрытый ключ (См. примечание выше).
# -keyout ca.key  Закрытый ключ сохранить в файл ca.key.
# -x509 Вместо создания CSR (см. опцию -new) создать самоподписанный сертификат.
# -days 500
# Срок действия сертификата 500 дней. Размер периода действия
# можете настроить по своему усмотрению. Не рекомендуется вводить
# маленькие значения, так как этим сертификатом вы будете
# подписывать клиентские сертификаты.
#  -subj /C=RU/ST=Msk/L=Msk/O=My\
#        Inc/OU=Sale/CN=bla/emailAddress=usr@dom.ru
# Данные сертификата, пары параметр=значение, перечисляются через '/'.
# Символы в значении параметра могут быть "подсечены" с
# помощью обратного слэша "\", например "O=My\ Inc". Также можно
#  взять значение аргумента в кавычки, например, -subj  "/xx/xx/xx".
# С - Двухсимвольный код страны (Country). Необязательный параметр.
# L - Название города/поселка/... (Locality Name). Необязательный параметр.
# O - Название организации (Organization Name). Необязательный параметр.      
# OU - Название отдела (Organization Unit). Необязательный параметр.
# CN - Имя сертификата, при создании серверных сертификатов
#       используется доменное имя сайта, для клиентских
#       сертификатов может быть использовано что угодно (Common
#       Name). Обязательный параметр. Максимальная длина 64 символа.
# emailAddress - почтовый адрес (E-mail address). 
# Необязательный параметр. Максимальная длина 40 символов.
#
# -out ca.crt Сертификат сохранить в файл ca.crt

# В результате выполнения команды появятся два файла ca.key и ca.crt.
# --------------------------------------------------------- #

echo "\n\n"
echo "Теперь нужно создать другой сертификат, подписанный корневым сертификатом."
echo "Создаем еще один ключ:"
#openssl genrsa -out user.key 2048

#----------------------------------------------------------------------#
 # Для создания подписанного клиентского сертификата предварительно
 # необходимо создать запрос на сертификат, для его последующей подписи.
 # Аргументы команды полностью аналогичны аргументам использовавшимся при
 # создании самоподписанного доверенного сертификата (см. $1), но
 # отсутсвует параметр -x509.

# ----------------------- клинет сертефикат ---------------------------#
openssl req -new -newkey rsa:2048 -nodes -keyout client01.key \
        -subj /C=RU/ST=Msk/L=Msk/O=Inc/OU=Web/CN=client/emailAddress=usr@dm.ru \
        -out client01.csr

#росмотреть данные закрытого ключа и запроса на сертификат (CSR) вы можете с помощью команд:

# openssl rsa -noout -text -in client01.key
# openssl req -noout -text -in client01.csr

#Подпись запроса на сертификат (CSR) с помощью доверенного сертификата (CA).
#openssl ca -config ca.config -in client01.csr -out client01.crt -batch
openssl x509 -req -in client01.csr -CA rootca.crt -CAkey rootca.key -CAcreateserial -out client01.crt -days 1000

#----------------------- конец клиент сертефикат ---------------------#

#-------------------------сервер сертефикат --------------------------#
openssl req -new -newkey rsa:2048 -nodes -keyout server01.key \
        -subj /C=RU/ST=Msk/L=Msk/O=Inc/OU=serv/CN=server/emailAddress=usr@dm.ru \
        -out server01.csr


#openssl ca -config ca.config -in server01.csr -out server01.crt -batch
openssl x509 -req -in server01.csr -CA rootca.crt -CAkey rootca.key -CAcreateserial -out server01.crt -days 1000

# ---------------------конец сервет сертефикат ------------------------#


# ca Подпись запроса с помощью CA.
# -config ca.config Использовать конфигурационный файл ca.config.
# -in client01.csr CSR находится в файле client01.csr
# -out client01.crt Сохранить сертификат в файл client01.crt
# -batch Не спрашивать подтверждения подписи.
# В результате выполнения команды появится файл клиентского сертификата
#  client01.crt. Просмотреть данные сертификата вы можете с помощью
#  команды:
# openssl x509 -noout -text -in client01.crt

# Для передачи полученных в результате предыдущих операций файлов
#  клиенту, обычно используется файл в формате PKCS#12. В этот файл
#  упаковывается и защищается паролем вся информация необходимая клиенту
#  для инсталяции сертификата в броузер.
 openssl pkcs12 -export -in client01.crt -inkey client01.key \
        -certfile rootca.crt -out client01.p12 -passout pass:q1w2e3



#echo ""
#echo "Создаем запрос на подпись, В интерактивном меню вам потребуется ответить на те же вопросы, что и при создании корневого сертификата. Нужно, чтобы введенный вами Common Name отличался от Common Name у корневого сертификата, это важно:"
#sleep 2s
#openssl req -new -key user.key -out user.csr
#echo ""
#echo "Теперь подписываем этот запрос корневым сертификатом:"
#openssl x509 -req -in user.csr -CA rootca.crt -CAkey rootca.key -CAcreateserial -out user.crt -days 20000

openssl verify -CAfile rootca.crt rootca.crt #OK
openssl verify -CAfile rootca.crt clietn01.crt #OK
openssl verify -CAfile user.crt user.crt #Bad

#echo "Создание Диффи-Хеллмана параметров:"
openssl dhparam -out dh2048.pem 2048

#echo "Ключ для корневого сертификата rootca.key, его нужно убрать подальше, на сервере он не нужен.
#    Корневой сертификат rootca.crt, он будет на сервере, а также будет распространятся вместе с приложением.
#    Ключ для рабочего сертификата user.key он будет хранится и использоваться на сервере.
#    Рабочий сертификат user.crt он будет хранится и использоваться на сервере.
#    dh2048.pem никак не связан с сертификатами, но он требуется для работы сервера и загружается при его создании."
