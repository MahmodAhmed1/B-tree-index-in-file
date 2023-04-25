#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <stdlib.h>
#include <string>
#include <iomanip>

using namespace std;

string convertCharToString(char* a, int size){
    int i;
    string s = "";
    for (i = 0; i < size; i++)
    {
        s += a[i];
    }
    return s;
}

vector<int> convertStringToVector(string record){
    vector<int> x = {};
    //cout<<record<<endl;

    if(record == "not found!")
    {
        cout << "not found!" << endl;
        return x;
    }

    else
    {
        string y="";
        for(int i = 0 ; i < record.size() && record[i]!='$'; i++)
        {
            while(record[i] != '|')
            {
                y += record[i];
                i++;
            }
            int num = stoi(y);
            x.push_back(num);
            y="";
        }
        return x;
    }

}

string convertVectorToString(vector<int> &record){

    string buffer="";
    for(int item : record)
    {
        string r =  to_string(item) ;
        buffer += r;
        buffer += "|";
    }
    buffer += '$';
    return buffer;
}

void swap(int *xp, int *yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
//0 , 23, 13, 4, 5, 3, 50, 8, 22, 12, 55
vector<int> selectionSort(vector<int> arr, int n){
    int i, j, min_idx;
    for (i = 1; i < n-1; i+=2)
    {
        min_idx = i;
        for (j = i+2; j < n; j+=2)
        {
            if(arr[j] != -1)
            {
                if (arr[j] < arr[min_idx])
                    min_idx = j;
            }

        }

        if(min_idx!=i)
            swap(&arr[min_idx], &arr[i]);
        swap(&arr[min_idx+1], &arr[i+1]);

    }
    return arr;
}

int countNode(vector<int> record){
    int counter=0;
    for(int i = 1 ; i<record.size(); i+=2)
    {
        if(record[i] != -1)
            counter++;
    }
    return counter;

}
// 0 -1 -1 2 33 4 88 8 99 -1 -1
vector<int> shiftVector(vector<int> node){
    for(int i = 1 ; i < node.size()-2 ; i+=2)
    {
        if(node[i] == -1)
        {
            swap(&node[i], &node[i+2]);
            swap(&node[i+1], &node[i+3]);
        }

    }
    return node;

}

class BTree
{
public:
    vector<string> BTreeIndex;
    vector<int> header;
    string finalRoot = "";
    bool flag = false;
    bool flag1 = false;
    int tempRecord = -1;
    int pos = 1;
    char* filename;
    int numberOfRecords;
    int m;

    BTree() {};

    BTree(char* filename, int numberOfRecords, int m) {
        this->filename = filename;
        this->numberOfRecords = numberOfRecords;
        this->m = m;
    };

    void DisplayIndexFileContent() {
        for(int i = 0; i < BTreeIndex.size(); i++)
        {
            vector<int> x = convertStringToVector(BTreeIndex[i]);
            cout<<setw(5);
            for(int j = 0; j < x.size(); j++)
            {
                cout<<x[j]<<setw(8);
            }
            cout<<endl;
        }
    }

    void CreateIndexFile (char* filename, int numberOfRecords, int m) {
        this->filename = filename;
        this->numberOfRecords = numberOfRecords;
        this->m = m;
        fstream file;
        string record;
        file.open(filename ,ios::binary | ios::out);

        for(int i = 1; i <= numberOfRecords ; i++)
        {
            for(int j = 0; j < m+1; j++)
            {
                if(j == 1 && i != numberOfRecords)
                {
                    record += to_string(i);
                    record += "|";
                }
                else
                {
                    record += "-1|";
                }
            }
            record.push_back('$');
            file << record << endl;
            BTreeIndex.push_back(record);
            record.clear();
        }
        header = convertStringToVector(BTreeIndex[0]);
        file.close();
    }

    void readFromFile() {
        fstream file;
        BTreeIndex.clear();
        file.open(filename, ios::binary | ios::in );
        int startRecord=0;
        for(int i = 0; i < 10; i++)
        {
            file.seekp(startRecord, file.beg);
            char buffer[100];
            file.getline(buffer, 100, '$');
            string x = convertCharToString(buffer,strlen(buffer));
            startRecord += strlen(buffer) + 1;
            BTreeIndex.push_back(x);
        }
        for(string item : BTreeIndex)
            cout<<item;

    }

    void writeToFile() {
        vector<string> index;
        fstream file;
        if(pos +1 == 11)
            header[1] = -1;
        else
            header[1] = pos + 1;

        BTreeIndex[0] = convertVectorToString(header);
        if(finalRoot.size()>2)
            BTreeIndex[1] = finalRoot;
        file.open(filename, ios::binary | ios::out);
        for(string item : BTreeIndex)
        {
            //item.pop_back();

            vector<int> x = convertStringToVector(item);
            for(int r:x)
                file<<r<<" ";
            file<<endl;
        }
//        for(string item : BTreeIndex)
//        {
//            vector<int> xxx = convertStringToVector(item);
//            for(int e : xxx){
//                e=char(e);
//                file.write((char*)&e, sizeof(e));
//
//                //file<< e <<" ";
//
//            }
//            file<<endl;
//
//        }
    }

    void InsertNewRecordAtIndex (int RecordID, int Reference) {
        vector <int> x;
        int tempSize = 0;
        int tempPos=0;
        int flag = 0;
        x = convertStringToVector(BTreeIndex[1]);

        if(x[0] == -1 || x[0] == 0)
            insertNewRecordAtRoot(RecordID, Reference, 0);

        else
        {
            for(int i = 1; i < x.size(); i+=2)
            {
                if(RecordID < x[i])
                {
                    tempPos = x[i+1];
                    break;
                }
            }
            if(tempPos == 0)
            {
                int temp = 0;

                if(x[x.size()-1] == -1)
                {
                    int temp = 0;
                    for(int j = 1; j < x.size(); j+=2)
                    {
                        if(x[j] != -1)
                        {
                            tempPos = x[j+1];
                            temp = j;
                        }
                    }
                    x[temp] = RecordID;

                    BTreeIndex[1] = convertVectorToString(x);
                }
                else
                {
                    flag = 1;
                    for(int j = 1; j < x.size(); j+=2)
                    {
                        if(x[j] != -1)
                        {
                            tempPos = x[j+1];
                        }
                    }
                }

            }

            vector<int> y;
            y = convertStringToVector(BTreeIndex[tempPos]);
            if(y[y.size()-1] != -1)
            {
                y.push_back(RecordID);
                y.push_back(Reference);
                y = selectionSort(y, y.size());
                split(convertVectorToString(y), tempPos, RecordID, Reference,flag);
            }
            else
            {
                for(int i = 3; i < y.size(); i++)
                {
                    if(y[i] == -1)
                    {
                        y[i] = RecordID;
                        y[i+1] = Reference;
                        i++;
                        break;
                    }
                }
                y = selectionSort(y, y.size());
                BTreeIndex[tempPos] = convertVectorToString(y);
            }
        }

        writeToFile();
    }

    void split(string record,int tempPos, int RecordID, int Reference, int flag) {
        vector<int> x;
        vector<int> nextNode;
        vector<int> root;
        int tempCurrentNode = 0;
        int tempNextNode = 0;
        x = convertStringToVector(record);
        root = convertStringToVector(BTreeIndex[1]);
        tempCurrentNode = x[x.size()-2];
        tempNextNode = x[x.size()/2-1];
        nextNode.push_back(0);

        for(int i = x.size()/2 + 1; i < x.size(); i++)
        {
            nextNode.push_back(x[i]);
            x[i] = -1;
        }
        x.pop_back();
        x.pop_back();

        for(int i = x.size()/2 + 1; i < x.size()-1; i++)
        {
            nextNode.push_back(-1);
        }
        if(root[root.size()-1] != -1 && flag1 == 1)
        {

            if(tempRecord > 0)
            {
                for(int i = 0; i < root.size(); i++)
                {
                    if(root[i] == tempRecord)
                    {
                        root[i] = tempCurrentNode;
                        root[i+1] = pos+1;
                        break;
                    }

                }
                BTreeIndex[1] = convertVectorToString(root);
                insertNewRecordAtRoot(tempNextNode, tempPos, flag);

                flag1 = 0;
            }

        }
        else
        {
            for(int i = 0; i < root.size(); i++)
            {

                if(root[i] == tempCurrentNode)
                    root[i+1] = pos+1;
                if(root[i] == -1)
                {
                    root[i] = tempNextNode;
                    root[i+1] = tempPos;
                    break;
                }
            }
        }

        if(root[root.size()-1] != -1)
        {
            tempRecord = tempCurrentNode;
            flag1 = 1;

        }


        pos++;

        root = selectionSort(root, root.size());
        BTreeIndex[1] = convertVectorToString(root);
        BTreeIndex[header[1]] = convertVectorToString(nextNode);
        BTreeIndex[tempPos] = convertVectorToString(x);
    }

    void insertNewRecordAtRoot(int RecordID, int Reference, int flag) {
        vector <int> x;
        x = convertStringToVector(BTreeIndex[1]);

        //check if need to splite
        if(x[x.size()-1] != -1)
        {
            x.push_back(RecordID);
            x.push_back(Reference);
            x = selectionSort(x, x.size());

            splitRoot(convertVectorToString(x), flag);
        }
        else
        {
            if(x[0] == -1)
            {

                x[1] = RecordID;
                x[2] = Reference;
                x[0] = 0;
            }
            else
            {
                for(int i = 3; i < x.size(); i++)
                {
                    if(x[i] == -1)
                    {
                        x[i] = RecordID;
                        x[i+1] = Reference;
                        i++;
                        break;
                    }
                }
            }
            x = selectionSort(x, x.size());
            BTreeIndex[pos] = convertVectorToString(x);

        }
    }

    void splitRoot(string record, int flag) {
        vector<int> x;
        vector<int> leftNode;
        vector<int> rightNode;
        int leftRecord = 0;
        int rightRecord = 0;
        x = convertStringToVector(record);

        int temp = 1;
        if(flag == 1)
            leftNode.push_back(1);
        else
            leftNode.push_back(0);

        for(int i = 1; i <= x.size()-3; i++)
        {
            if(i <= (x.size()-1)/2)
            {
                leftNode.push_back(x[i]);
                leftRecord = x[i];
                i++;
                leftNode.push_back(x[i]);
                temp+=2;
            }
            else
            {
                leftNode.push_back(-1);
            }
        }


        if(flag == 1)
            rightNode.push_back(1);
        else
            rightNode.push_back(0);
        for(int j = temp; j < (x.size()-3) + temp; j++)
        {
            if(j < x.size())
            {
                rightNode.push_back(x[j]);
                rightRecord = x[j];
                j++;
                rightNode.push_back(x[j]);
            }
            else
                rightNode.push_back(-1);
        }

        if(flag == 1)
            pos++;
        x.clear();
        x.push_back(1);
        pos++;
        BTreeIndex[pos] = convertVectorToString(leftNode);
        x.push_back(leftRecord);
        x.push_back(pos);


        pos++;
        x.push_back(rightRecord);
        x.push_back(pos);

        BTreeIndex[pos] = convertVectorToString(rightNode);
        int a = x.size();
        for(int i = a; i < leftNode.size(); i++)
            x.push_back(-1);
        if(flag==1)
            finalRoot = convertVectorToString(x);
        else
            BTreeIndex[1] = convertVectorToString(x);
    }

    int SearchARecord(int RecordID) {
        //12
        vector<int> root;
        int temp=0;
        root = convertStringToVector(BTreeIndex[1]);
        //cout<<convertVectorToString(root)<<endl;

        if(RecordID > 0)
        {
            if(root[0] == 1)
            {
                while(temp != -1)
                {
                    for(int i=1; i<root.size(); i+=2)
                    {
                        if(root[i] >= RecordID)
                        {
                            temp = root[i+1];
                            break;
                        }
                    }
                    root = convertStringToVector(BTreeIndex[temp]);
                    //cout<<temp<<" "<< convertVectorToString(root)<<endl;
                    if(root[0] != 1 )
                    {
                        temp = -1;
                        for(int j = 1; j < root.size(); j+=2)
                        {
                            if(root[j] == RecordID)
                            {
                                return root[j+1];
                            }
                        }
                        return -1;
                    }


                }
            }
            else if(root[0] == 0)
            {
                for(int j = 1; j < root.size(); j+=2)
                {
                    if(root[j] == RecordID)
                    {
                        return root[j+1];
                    }
                }
            }
            else
                return -1;
        }
        else
            return -1;
    }

    void DeleteRecordFromIndex (int RecordID) {
        vector<int> rootTemp;
        rootTemp = convertStringToVector(BTreeIndex[1]);

        if(rootTemp[0] == 1){
            if(SearchARecord(RecordID) != -1){
                vector<int> pathOfRecord;
                vector<int> DLT;
                vector<int> tempNode;
                bool flag = false;
                int tempR = -1;
                int tempRef = -1;
                pathOfRecord = findPathToDelete(RecordID);
                for(int i = pathOfRecord.size()-2; i >= 0; i--)
                {
                    DLT = convertStringToVector(BTreeIndex[pathOfRecord[i]]);

                    if(DLT[0] == 0)
                    {
                        for(int j = 1 ; j < DLT.size(); j+=2)
                        {

                            if(DLT[i] != -1)
                            {
                                if(DLT[j] == RecordID)
                                {
                                    DLT[j] = -1;
                                    DLT[j+1] = -1;
                                    DLT = shiftVector(DLT);
                                    if((m/4) <= countNode(DLT))
                                    {
                                        tempR = DLT[j-2];
                                        BTreeIndex[pathOfRecord[i]] = convertVectorToString(DLT);
                                        break;
                                    }
                                    else
                                    {

                                        tempNode = convertStringToVector(BTreeIndex[pathOfRecord[pathOfRecord.size()-1]]);
                                        if((m/4) < countNode(tempNode))
                                        {
                                            tempR = DLT[j-2];
                                            int recordpos = countNode(tempNode)*2-1;
                                            int referencepos = countNode(tempNode)*2;
                                            DeleteRecordFromIndex(tempNode[recordpos]);
                                            DLT.insert(DLT.begin() + 1, tempNode[recordpos]);
                                            DLT.insert(DLT.begin() + 2, tempNode[referencepos]);
                                            DLT.pop_back();
                                            DLT.pop_back();
                                            BTreeIndex[pathOfRecord[i]] = convertVectorToString(DLT);
                                        }
                                        else
                                        {
                                            flag = true;
                                            tempR = DLT[j-2];
                                            tempRef = pathOfRecord[pathOfRecord.size()-1];
                                            //cout<<DLT[j-2]<<endl;
                                            int recordpos = countNode(DLT)*2-1;
                                            int referencepos = countNode(DLT)*2;
                                            tempNode.insert(tempNode.begin() + 1, DLT[recordpos]);
                                            tempNode.insert(tempNode.begin() + 2, DLT[referencepos]);
                                            tempNode = selectionSort(tempNode, tempNode.size());
                                            for(int k = 0 ; k < DLT.size(); k++)
                                            {
                                                DLT[k] = -1;
                                            }
                                            header[1] = pathOfRecord[i];
                                            BTreeIndex[0] = convertVectorToString(header);
                                            BTreeIndex[pathOfRecord[pathOfRecord.size()-1]] = convertVectorToString(tempNode);
                                            BTreeIndex[pathOfRecord[i]] = convertVectorToString(DLT);
                                        }

                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        for(int j = 1 ; j < DLT.size(); j+=2)
                        {
                            if(DLT[j] == RecordID)
                            {
                                if(flag == true && DLT != convertStringToVector(BTreeIndex[1]))
                                    DLT[j+1] = tempRef;
                                DLT[j] = tempR;
                                BTreeIndex[pathOfRecord[i]] = convertVectorToString(DLT);
                                break;
                            }
                        }
                    }
                }
                fstream file;
                file.open(filename, ios::binary | ios::out);
                for(string item : BTreeIndex){
                    item.pop_back();
                    file<<item<<endl;
                }
            }
        }
        else if(rootTemp[0] == 0)
        {
            for(int i = 1; i < rootTemp.size() ; i+=2)
            {
                if(rootTemp[i] == RecordID)
                {
                    rootTemp[i] = -1;
                    rootTemp[i+1] = -1;
                    rootTemp = shiftVector(rootTemp);
                    if(rootTemp[1] == -1)
                    {
                        rootTemp[0] = -1;
                        header[1] = 1;
                        rootTemp[1] = 2;
                        BTreeIndex[0] = convertVectorToString(header);
                    }

                    BTreeIndex[1] = convertVectorToString(rootTemp);
                    break;
                }
            }
            fstream file;
            file.open(filename, ios::binary | ios::out);
            for(string item : BTreeIndex)
            {
                file<<item<<endl;
            }
        }
        else
            cout<<"error....."<<endl;
    }

    vector<int> findPathToDelete(int RecordID) {
        vector<int> root;
        int temp = 1;
        int temp1 = 0;
        int x = 1;
        vector<int> tempVector;
        root = convertStringToVector(BTreeIndex[1]);
        if(SearchARecord(RecordID) > 0)
        {
            if(RecordID > 0)
            {
                while(temp != -1)
                {
                    for(int i=1; i<root.size(); i+=2)
                    {
                        if(root[i] >= RecordID)
                        {
                            if(root[i] == RecordID && root[0] != 0)
                                temp1 = 1;
                            if(root[0] != 0 )
                            {
                                if(i == 1)
                                    x = root[i+3];
                                else if(i > 1)
                                    x = root[i-1];
                                temp = root[i+1];
                                tempVector.push_back(temp);
                                break;
                            }
                        }
                    }
                    if(root[0] == 0 )
                    {
                        temp = -1;
                        tempVector.push_back(x);
                        if(temp1 == 1)
                            tempVector.insert(tempVector.begin() + 0, temp1);
                        return tempVector;
                    }
                    else
                        root = convertStringToVector(BTreeIndex[temp]);
                }
            }
        }
        else{
            tempVector.clear();
            return tempVector;
        }
    }

};



int main()
{
    BTree bTree;

    string f;
    cout << "Enter the file name" << endl;
    cin >> f;
    f += ".txt";
    char *fileName = &f[0];

    bTree.CreateIndexFile(fileName, 10, 10);
    cout<<endl;

    int choice = -1;

    do{
        cout<<"Insert new record at index press 1"<<endl;
        cout<<"Delete record from index press 2"<<endl;
        cout<<"Display index file content press 3"<<endl;
        cout<<"Search a record press 4"<<endl;
        cout<<"exit press 0\n"<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;
        switch(choice)
        {
        case 1:
            int y;
            cout << "Insert....." << endl;
            cout << "use test case press 1" << endl;
            cout << "insert new record press 2" << endl;
            cout<<"Enter your choice: ";
            cin >> y;
            if(y == 1) {
                bTree.InsertNewRecordAtIndex (3, 12);
                bTree.InsertNewRecordAtIndex (7, 24);
                bTree.InsertNewRecordAtIndex (10, 48);
                bTree.InsertNewRecordAtIndex (24, 60);
                bTree.InsertNewRecordAtIndex (14, 72);

                bTree.InsertNewRecordAtIndex (19, 84);
                bTree.InsertNewRecordAtIndex (30, 196);
                bTree.InsertNewRecordAtIndex (15, 108);
                bTree.InsertNewRecordAtIndex (1, 120);
                bTree.InsertNewRecordAtIndex (5, 132);

                bTree.InsertNewRecordAtIndex (2, 144);
                bTree.InsertNewRecordAtIndex (8, 156);
                bTree.InsertNewRecordAtIndex (9, 168);
                bTree.InsertNewRecordAtIndex (6, 180);
                bTree.InsertNewRecordAtIndex (11, 192);

                bTree.InsertNewRecordAtIndex (12, 204);
                bTree.InsertNewRecordAtIndex (17, 216);
                bTree.InsertNewRecordAtIndex (18, 228);
                bTree.InsertNewRecordAtIndex (32, 240);
                cout << "Insert Successfully...\n" << endl;
            }

            else if(y == 2) {
                int r, re;
                cout << "Enter the record: ";
                cin >> r;
                cout << "Enter the reference: ";
                cin >> re;
                bTree.InsertNewRecordAtIndex (r, re);
                cout << "Insert Successfully...\n" << endl;
            }
            break;

        case 2:
            int d;
            cout << "Delete....." << endl;
            cout << "enter the record: ";
            cin >> d;
            if(bTree.SearchARecord(d) > 0) {
                bTree.DeleteRecordFromIndex(d);
                cout << "Delete Successfully...\n" << endl;
            }
            else
                cout << "Record is not found...\n" << endl;
            break;

        case 3:
            cout << "Display Index File Content....." << endl;
            bTree.DisplayIndexFileContent();
            cout << endl;
            break;

        case 4:
            int x;
            cout << "Search....." << endl;
            cout << "enter the record: ";
            cin >> x;
            if(bTree.SearchARecord(x) == -1)
                cout << "Record is not found......\n" << endl;
            else{
                cout << "Record is Founded" << endl;
                cout << "Reference of record is:  " << bTree.SearchARecord(x) << "\n" << endl;
            }

            break;

        default:
            cout << "enter the correct choice..........\n" << endl;
        }
    } while(choice != 0);

//    bTree.InsertNewRecordAtIndex (3, 12);
//    bTree.InsertNewRecordAtIndex (7, 24);
//    bTree.InsertNewRecordAtIndex (10, 48);
//    bTree.InsertNewRecordAtIndex (24, 60);
//    bTree.InsertNewRecordAtIndex (14, 72);
//
//    bTree.InsertNewRecordAtIndex (19, 84);
//    bTree.InsertNewRecordAtIndex (30, 196);
//    bTree.InsertNewRecordAtIndex (15, 108);
//    bTree.InsertNewRecordAtIndex (1, 120);
//    bTree.InsertNewRecordAtIndex (5, 132);
//    //
//    bTree.InsertNewRecordAtIndex (2, 144);
//    bTree.InsertNewRecordAtIndex (8, 156);
//    bTree.InsertNewRecordAtIndex (9, 168);
//    bTree.InsertNewRecordAtIndex (6, 180);
//    bTree.InsertNewRecordAtIndex (11, 192);
//    //
//    bTree.InsertNewRecordAtIndex (12, 204);
//    bTree.InsertNewRecordAtIndex (17, 216);
//    bTree.InsertNewRecordAtIndex (18, 228);
//    bTree.InsertNewRecordAtIndex (32, 240);
//
//    cout<<"\n"<<bTree.SearchARecord(10)<<endl;
//    bTree.DeleteRecordFromIndex(10);
//    cout<<bTree.SearchARecord(10)<<endl;
//    bTree.DeleteRecordFromIndex(7);
//    bTree.DeleteRecordFromIndex(7);
//    bTree.DisplayIndexFileContent();
//
//    cout<<"\n"<<endl;


    return 0;
}
