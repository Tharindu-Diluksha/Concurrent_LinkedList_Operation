//have used g++ 11 version for all the compilations
//include -g flag if needed debug information (GDB)

//to run each one provide the number of sample cases and case number followed by the program name

=============================== Serial ============================

//to compile serial version
g++ -std=c++11 linked_list_serial.cpp -o serial

//to run  
./serial NumberOFSamples CaseNumber
=============================End Serial ===========================



=============================== Mutex =============================

//to compile mutex version
g++ -std=c++11 -pthread linked_list_one_mutex.cpp -o mutex

//to run
./mutex NumberOFSamples CaseNumber

==============================End Mutex ===========================



========================= Read Write Lock =========================

// to compile rw_lock version
g++ -std=c++11 -pthread linked_list_rw_lock.cpp -o rwlock

// to run
./rwlock NumberOFSamples CaseNumber

========================End Read Write Lock =======================

