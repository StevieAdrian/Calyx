#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<stdbool.h>

int map_size;

struct Leaderboard{
    int score;
    char name[16],id[6];
}PlayerData[1000],CurrentPlayer;

struct Cell{
    int value,Lspace,Rspace;
}Grid[100][100];

HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);

void pause(){
    printf("Press any key to continue. . .\n");
    getch();
}
int GeneratePlayerId(){
    int res;
    int rng,rng2;
    int i;
    rng=rand() % 2;
    if(rng==1){
        res = rand()%10 + '0';
    }else if(rng==0){
        rng2=rand()%2;
        if(rng2==0){
            res = rand()%26 + 'a';
        }else{
            res = rand()%26 + 'A';
        }

    }
    return res;
}

void SetColor(int value){
    if(value == 2)SetConsoleTextAttribute(hc,0x37);
    else if(value == 4)SetConsoleTextAttribute(hc,0xc7);
    else if(value == 8)SetConsoleTextAttribute(hc,0xD7);
    else if(value == 16)SetConsoleTextAttribute(hc,0x57);
    else if(value == 32)SetConsoleTextAttribute(hc,0xB7);
    else if(value == 64)SetConsoleTextAttribute(hc,0x67);
    else if(value == 128)SetConsoleTextAttribute(hc,0xA7);
    else if(value == 256)SetConsoleTextAttribute(hc,0x97);
    else if(value == 512)SetConsoleTextAttribute(hc,0x17);
    else if(value == 1024)SetConsoleTextAttribute(hc,0x47);
    else if(value == 2048)SetConsoleTextAttribute(hc,0x27);
    else SetConsoleTextAttribute(hc,0x07);
}

void Initialize(int size){
    int i,j;
    for(i =0;i<size;i++){
        for(j=0;j<size;j++){
            Grid[i][j].value=0;
        }
    }
}

void GetCellSpaces(struct Cell *ptr){
    int len=0,temp= ptr->value;
    while(temp > 0){
        temp/=10;len++;
    }
    if(len == 0){
        ptr->Lspace = 4;
        ptr->value=0;
        ptr->Rspace=0;
    }else if(len==1){
        ptr->Lspace=1;
        ptr->Rspace=2;
    }else if(len==2){
        ptr->Lspace=1;
        ptr->Rspace=1;
    }else if(len==3){
        ptr->Lspace=0;
        ptr->Rspace=1;
    }else{
        ptr->Lspace=0;
        ptr->Rspace=0;
    }
}

void PrintCellValue(Cell A){
    int i;
    for(i=0;i<A.Lspace;i++)printf(" ");
    if(A.value>0)printf("%d",A.value);
    for(i=0;i<A.Rspace;i++)printf(" ");
}

char MainMenu(){
    char in;
    system("CLS");
    printf("             2048            \n");
    printf("=============================\n");
    printf("        (1) Play\n");
    printf("        (2) Leaderboard\n");
    printf("        (3) Exit\n");
    in = getch();
    return in;

}

int GetMapSize(){
    int n;
    while(1){
        system("CLS");
        printf("Input map size (4/6): ");
        if(scanf("%d",&n) && (n == 4 || n == 6))return n;
        else printf("Error, wrong input type!\nPress any button to continue. . .\n");
        getch();
    }
}

void PrintMap(int size){
    system("CLS");
    int i,j,countRow=0,countCol=0;
    int CellRow=0,CellCol=0;
    int rowborder1=0,rowborder2=4;
    int colborder1=0,colborder2=5;
    int valuerow=2,value_edge=1;
    for(i=0;i<5*size;i++){
        CellCol=0;
        value_edge=1;
        colborder1=0;colborder2=5;
        if(i !=valuerow){
            for(j=0;j<6*size;j++){
                if(i==rowborder1 && (j != colborder1 || j!=colborder2)){
                    SetConsoleTextAttribute(hc,0x77);
                    printf(" ");
                }else if(i==rowborder2 && (j != colborder1 || j!=colborder2)){
                    SetConsoleTextAttribute(hc,0x77);
                    printf(" ");
                }else if(j==colborder1){
                    SetConsoleTextAttribute(hc,0x77);
                    colborder1+=6;
                    printf(" ");
                }else if(j==colborder2){
                    SetConsoleTextAttribute(hc,0x77);
                    colborder2+=6;
                    printf(" ");
                }else{
                    SetColor(Grid[CellRow][CellCol].value);
                    printf(" ");
                    countCol++;
                    if(countCol == 4){
                        countCol=0;
                        CellCol++;
                    }
                }
            }
        }else{
            for(j=0;j<6*size;j++){
                if(j==colborder1){
                    SetConsoleTextAttribute(hc,0x77);
                    colborder1+=6;
                    printf(" ");
                }else if(j==colborder2){
                    SetConsoleTextAttribute(hc,0x77);
                    colborder2+=6;
                    printf(" ");
                }else if(j==value_edge){
                    SetColor(Grid[CellRow][CellCol].value);
                    GetCellSpaces(&Grid[CellRow][CellCol]);
                    PrintCellValue(Grid[CellRow][CellCol]);
                    CellCol++;
                    value_edge+=6;
                }
            }
            valuerow+=5;
        }
        printf("\n");
        if(i!=rowborder1 && i!=rowborder2)countRow++;
        if(countRow==3){
            countRow=0;
            CellRow++;
        }
        if(i==rowborder1)rowborder1+=5;
        else if(i==rowborder2)rowborder2+=5;
    }
    SetConsoleTextAttribute(hc,0x07);
}

void SwapCell(Cell *A,Cell *B){
    Cell Temp=*A;
    *A=*B;
    *B=Temp;
}

int MergeCell(Cell *A,Cell *B){
    int new_value;
    new_value = A->value+B->value;
    A->value = new_value;
    B->value=0;
    return new_value;

}

void NumberSpawn(int size){
    int rand_x,rand_y,rng=rand()%2;
    do{
        rand_x = rand() % size;
        rand_y = rand() % size;
    }while(Grid[rand_x][rand_y].value != 0);
    if(rng == 1){
        Grid[rand_x][rand_y].value = 2;
    }else{
        Grid[rand_x][rand_y].value = 4;
    }
}

int Move(char direction,int size,int *score){
    int row,col,i,moved=0;
    if(direction == 'w'){ //KE ATAS
        for(col=0;col<size;col++){
            for(row=0;row<size;row++){
                if(Grid[row][col].value == 0){//JIKA KOSONG
                    for(i=row+1;i<size;i++){
                        if(Grid[i][col].value > 0){
                            SwapCell(&Grid[row][col],&Grid[i][col]);
                            row--;
                            moved=1;
                            break;
                        }
                    }
                }else{
                    for(i=row+1;i<size;i++){
                        if(Grid[row][col].value == Grid[i][col].value && Grid[i][col].value > 0){
                            *score += MergeCell(&Grid[row][col],&Grid[i][col]);
                            moved=1;
                            break;
                        }else if(Grid[row][col].value != Grid[i][col].value && Grid[i][col].value > 0){
                            break;
                        }
                    }
                }

            }
        }
    }else if(direction == 's'){ //KE BAWAH
        for(col=0;col<size;col++){
            for(row=size-1;row>=0;row--){
                if(Grid[row][col].value == 0){//JIKA KOSONG
                    for(i=row-1;i>=0;i--){
                        if(Grid[i][col].value > 0){
                            SwapCell(&Grid[row][col],&Grid[i][col]);
                            row++;
                            moved=1;
                            break;
                        }
                    }
                }else{
                    for(i=row-1;i>=0;i--){
                        if(Grid[row][col].value == Grid[i][col].value && Grid[i][col].value > 0){
                            *score += MergeCell(&Grid[row][col],&Grid[i][col]);
                            moved=1;
                            break;
                        }else if(Grid[row][col].value != Grid[i][col].value && Grid[i][col].value > 0){
                            break;
                        }
                    }
                }

            }
        }
    }else if(direction == 'd'){ //KE KANAN
        for(row=0;row<size;row++){
            for(col=size-1;col>=0;col--){
                if(Grid[row][col].value == 0){ //JIKA KOSONG
                    for(i=col-1;i>=0;i--){
                        if(Grid[row][i].value > 0){
                            SwapCell(&Grid[row][col],&Grid[row][i]);
                            moved=1;
                            col++;
                            break;
                        }
                    }
                }else{
                    for(i=col-1;i>=0;i--){
                        if(Grid[row][col].value == Grid[row][i].value && Grid[row][i].value > 0){
                            *score += MergeCell(&Grid[row][col],&Grid[row][i]);
                            moved=1;
                            break;
                        }else if(Grid[row][col].value != Grid[row][i].value && Grid[row][i].value > 0){
                            break;
                        }
                    }
                }

            }
        }
    }else if(direction == 'a'){ //KE KIRI
        for(row=0;row<size;row++){
            for(col=0;col<size;col++){
                if(Grid[row][col].value == 0){//JIKA KOSONG
                    for(i=col+1;i<size;i++){
                        if(Grid[row][i].value > 0){
                            SwapCell(&Grid[row][col],&Grid[row][i]);
                            moved=1;
                            col--;
                            break;
                        }
                    }
                }else{
                    for(i=col+1;i<size;i++){
                        if(Grid[row][col].value == Grid[row][i].value && Grid[row][i].value > 0){
                            *score += MergeCell(&Grid[row][col],&Grid[row][i]);
                            moved=1;
                            break;
                        }else if(Grid[row][col].value != Grid[row][i].value && Grid[row][i].value > 0){
                            break;
                        }
                    }
                }

            }
        }
    }
    return moved;
}

int WinCheck(int size){
    int i,j;
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            if(Grid[i][j].value >= 2048)return 1;
        }
    }
    return 0;
}

int LoseCheck(int size){
    int total_cell=size*size,filled_cell_counter=0,possible=0;
    int i,j;
    Cell temp;
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            if(Grid[i][j].value > 0)filled_cell_counter++;
        }
    }
    if(filled_cell_counter == total_cell){
        for(i =0;i<size;i++){ //Check Column
            for(j=0;j<size-1;j++){
                if(Grid[i][j].value == Grid[i][j+1].value)return 0;
            }
        }
        for(i =0;i<size;i++){ //Check Row
            for(j=0;j<size-1;j++){
                if(Grid[j][i].value == Grid[j+1][i].value)return 0;
            }
        }
        return 1;
    }
    return 0;
    
    
}

void EnterPlayerData(int score){
    int i;
    getchar();
    for(i=0;i<5;i++){
        CurrentPlayer.id[i]=GeneratePlayerId();
    }
    CurrentPlayer.score = score;
    system("CLS");
    printf("SCORE : %d\n",score);
    printf("Enter Name [max 15 char]\n");
    scanf("%s",CurrentPlayer.name);
}

void SwapPlayerData(Leaderboard *A,Leaderboard *B){
    Leaderboard Temp = *A;
    *A = *B;
    *B = Temp;
}

int PartitionPlayerData(Leaderboard A[],int low,int high){
    int pivot=A[high].score;
    int j,i=low-1;
    for(j=low;j<high;j++){
        if(A[j].score > pivot){
            i++;
            SwapPlayerData(&A[i],&A[j]);
        }
    }i++;
    SwapPlayerData(&A[i],&A[high]);
    return i;
}

void QuickSortPlayerData(Leaderboard arr[],int low,int high){
    int pivot_idx;
    if(low < high){
        pivot_idx = PartitionPlayerData(arr,low,high);
        QuickSortPlayerData(arr,low,pivot_idx-1);
        QuickSortPlayerData(arr,pivot_idx+1,high);
    }
}

int CheckPlayerDataTotal(){
    int i=0;
    while(strlen(PlayerData[i].name)!=0){
        i++;
    }
    return i;
}

void SavePlayerData(){
    FILE *f;
    int a,i,j=CheckPlayerDataTotal();
    f = fopen("highscore.txt","w+");
    for(i=0;i<j;i++){
        fprintf(f,"%s %s %d\n",PlayerData[i].id,PlayerData[i].name,PlayerData[i].score);
    }
    fclose(f);
}
void LoadPlayerData(){
    FILE *f;
    int flag_id=0,flag_nama=0;
    int i=0,j=0;
    f = fopen("highscore.txt","r+");
    if(f != NULL){
        while(!feof(f)){
            fscanf(f,"%s %s %d\n",&PlayerData[i].id,&PlayerData[i].name,&PlayerData[i].score);
            i++;
        }
    }
    fclose(f);
}

void PrintLeaderBoard(int size){
    int i,spaces,mid;
    int n;
    if(size > 10)n=10;
    else n=size;
    for(i=0;i<n;i++){
        if(i+1 < 10){
            printf("%d  | ",i+1);
        }else printf("%d | ",i+1);
        printf("%s |",PlayerData[i].id);
        spaces = 15-strlen(PlayerData[i].name);
        mid = spaces/2;
        while(spaces > mid){
            printf(" ");
            spaces--;
        }printf("%s",PlayerData[i].name);
        while(spaces > 0){
            printf(" ");
            spaces--;
        }
        printf("| ");
        printf("%d\n",PlayerData[i].score);
    }
}

int main(){
    srand(time(0));
    char menu,move;
    int newnumber,score=0,i;
    int donePlaying=0,total_data=0;
    LoadPlayerData();
    total_data=CheckPlayerDataTotal();
    do{
        score=0;
        donePlaying=0;
        menu = MainMenu();
        if(menu == '1'){
            map_size = GetMapSize();
            Initialize(map_size);
            NumberSpawn(map_size);
            while(1){
                PrintMap(map_size);
                printf("\nSCORE : %d\n",score);
                printf("[PRESS ENTER TO EXIT]\n");
                move=getch();
                newnumber = Move(move,map_size,&score);
                if(newnumber)NumberSpawn(map_size);
                if(WinCheck(map_size)){
                    system("CLS");
                    PrintMap(map_size);
                    printf("FINAL SCORE : %d\n",score);
                    printf("YOU WIN!\n");
                    getch();
                    donePlaying=1;
                }
                else if(LoseCheck(map_size)){
                    system("CLS");
                    PrintMap(map_size);
                    printf("FINAL SCORE : %d\n",score);
                    printf("YOU LOSE\n");
                    getch();
                    donePlaying=1;
                }
                if(donePlaying){
                    EnterPlayerData(score);
                    total_data++;
                    PlayerData[total_data]=CurrentPlayer;
                    QuickSortPlayerData(PlayerData,0,total_data);
                    SavePlayerData();
                    break;
                }else if(move == '\r'){
                    break;
                }
            }
        }else if(menu == '2'){ 
            system("CLS");
            printf("NO |  ID   |     Names     | Score\n");
            printf("======================================\n");
            QuickSortPlayerData(PlayerData,0,total_data);
            SavePlayerData();
            PrintLeaderBoard(total_data);
            pause();
        }
    }while(menu != '3');
    return 0;
}
