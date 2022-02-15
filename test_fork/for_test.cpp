#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include <iostream>
#include <fstream>



/* типовая функция для ухода в демона,
найдена на
https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux */

/* или ссылка на https://github.com/pasce/daemon-skeleton-linux-c */

// пример .
static void daemon() {
  pid_t pid;

  /* форк от родительского процесса */
  pid = fork();

  /* ошибка */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* разрешаем завершить работу родителя  */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  /* Успех: дочерний процесс станет лидером сессии*/
  if (setsid() < 0) {
    exit(EXIT_FAILURE);
  }
  /* Catch, ignore and handle signals */
  // TODO: Implement a working signal handler */

  /*ловим и обрабатываем сигналы, */
  /* обработка здесь */
 
 /* здесь заблокирую */
//  signal(SIGCHLD, SIG_IGN);
//  signal(SIGHUP, SIG_IGN);

  /* повторно делаем Fork  отвязываемся от процесса ведущего сеанс*/
  pid = fork();

  /* ошибка */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* разрешаем завершить работу родителя */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }
  /* змените маску режима файла в соотвествии с потребностями */
  umask(0);

  /* изменяем рабочую директорию на корневую  */
  /* или другой каталог */
  chdir("/");

  /* закрываем открытые дискрипторы */
  int x;
  for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
    close(x);
  }

  /* Open the log file */
  openlog("wfm-parser-daemon", LOG_PID, LOG_DAEMON);
}

// тут я пробую обработку сигналов после многократного fork();

bool terminate = false;

void HandleSignal(int signal) {
  std::cout << std::endl << "SignalHandled: " << signal << std::endl;
  ::terminate = true;
}

int main(int argc, const char* argv[]) {
    std::cout << "i`m run " << std::endl; 


    // и здесь работает . 
   signal(SIGINT, HandleSignal);
   signal(SIGTERM, HandleSignal);

    daemon();
    daemon();
    daemon();


// вот тут обработчик срабатывает . 
 //   signal(SIGINT, HandleSignal);
 //   signal(SIGTERM, HandleSignal);

    int cycle = 50;

    std::ofstream out;          // поток для записи
    out.open(argv[1]); 
    int pid = getpid();

    while (true) {

        if (terminate) {
             if (out.is_open())
            {
                out << "Terminate =- true !" << pid << std::endl;
            }
        } else {
            out << "Terminate =- FALSE !" << pid << std::endl;
        }
        --cycle;
        sleep(2);
        if (cycle == 0 ) {
            out << "exit" << std::endl;
            exit(0);
        }
    }


}