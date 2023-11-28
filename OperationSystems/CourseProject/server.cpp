#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <vector>
#include <fcntl.h>
#include "functions.hpp"
#include <map>
#include <vector>

int in(std::vector<std::string> logins, std::string str)
{
    for (int i = 0; i < logins.size(); ++i)
    {
        if (logins[i] == str)
            return i;
    }
    return -1;
}

int main()
{
    std::vector<std::vector<std::string> > history; // здесь хранится история переписок
    // соответсвие с вектором logins по принципу ассоциативных векторов
    std::vector<std::string> logins;
    std::string command;
    std::string login;

    //ввод логинов
    std::cout << "Enter all user's logins. Insert 'end' to stop:\n";
    while (login != "end")
    {
        std::cin >> login;
        std::vector<std::string> vec;
        vec.push_back(login);
        history.push_back(vec);
        if (in(logins, login) == -1)
            logins.push_back(login);
        else
            std::cout << "already exists!";
    }
    std::cout << "ALL RIGHT! CHAT IS READY. LAUNCH CLIENTS\n";
    //создание выходных FIFO для всех логинов
    for (int i = 0; i < logins.size(); ++i)
    {
        if (mkfifo(logins[i].c_str(), 0777) == -1)
        {
            if (errno != EEXIST)
            {
                std::cout << "FIFO WAS NOT CREATED";
                exit(1);
            }
        }
    }
    //создание входного FIFO
    if (mkfifo("input", 0777) == -1)
    {
        std::cout << "MAIN INPUT FIFO WAS NOT CREATED";
        exit(1);
    }
    int fd_recv = open("input", O_RDWR);
    if (fd_recv == -1)
    {
        std::cout << "INPUT FIFO WAS NOT OPENED";
        exit(1);
    }

    //открытие всех FIFO на запись
    int fd[logins.size()];
    for (int i = 0; i < logins.size(); ++i)
    {
        fd[i] = open(logins[i].c_str(), O_RDWR);
    }

    while (true)
    {

        std::string message;
        message = recieve_message_server(fd_recv);
        std::cout << message;
        std::string rcvd_usr = extract_login(message);          //от кого
        std::string rcvd_adressee = extract_addressee(message); //кому
        std::string rcvd_message = extract_message(message);    //что
        int fd_repl = in(logins, rcvd_adressee);                //id получателя
        int fd_usr = in(logins, rcvd_usr);                      //id отправителя
        //std::cout << rcvd_adressee;
        int pos = -1;
        if (rcvd_adressee == "history")
        {
            std::string reply = "No matches found\n";
            for (int i = 0; i < history.size(); ++i)
            {
                if (i == fd_usr)
                {
                    for (int j = 0; j < history[i].size(); ++j)
                    {
                        if (search(history[i][j], extract_text(message)))
                        {
                            reply = history[i][j];
                            pos = i;
                        }
                    }
                }
            }
            if (reply != "No matches found\n")
            {
                for (int i = 0; i < history.size(); ++i)
                {
                    if (i != pos)
                    {
                        for (int j = 0; j < history[i].size(); ++j)
                        {
                            if (search(history[i][j], extract_text(message)))
                            {
                                reply = "[To " + logins[i] + "] " + history[i][j];
                            }
                        }
                    }
                }
            }
            send_message_to_client(fd[fd_usr], reply);
        }
        else
        {
            for (int i = 0; i < history.size(); ++i)
            {
                if (logins[i] == rcvd_usr)
                    history[i].push_back(extract_text(message));
                if (logins[i] == rcvd_adressee && rcvd_usr != rcvd_adressee)
                    history[i].push_back(extract_text(message));
            }
            // for (int i = 0; i < history.size(); ++i)
            // {
            //     for (int j = 0; j < history[i].size(); ++j)
            //     {
            //         std::cout << history[i][j] << " ";
            //     }
            //     std::cout << "\n";
            // }
            if (in(logins, rcvd_adressee) == -1)
            {
                send_message_to_client(fd[fd_usr], "Login does not exists!\n");
            }
            else
            {
                send_message_to_client(fd[fd_repl], rcvd_message);
            }
        }
    }
}
