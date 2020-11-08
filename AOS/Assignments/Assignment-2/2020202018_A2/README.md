# AOS ASSIGNMENT - 2

### tracker.cpp:
- g++ -o tracker tracker.cpp -lpthread
- ./tracker tracker_info.txt 1

### peer.cpp:
- g++ -o peer peer.cpp -lpthread
- ./peer 127.0.0.1 5000 tracker_info.txt
- **Commands:**
    - **create_user** suman abc
    - **login** suman abc
    - **create_group** g1
    - **fetch_groups**
    - **request_group_join** g1
    - **upload_file** 1.txt
    - **show_files**
    - **download_file** 1.txt
    - **logout**
    - **quit**