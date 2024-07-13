
#include <stdio.h>
#pragma warning (disable:4996)

/*
1401고은혁 _ 라운드 로빈 스케줄링 대기시간,반환시간 계산 프로그램
프로세스 5개

	**반복 알고리즘**
	도착하지 않은 프로세스가 있다면
		도착 확인, 큐에 추가
	이전 프로세스 종료/time run out 발생시
		시간할당량 카운트 리셋
		큐 업데이트 (삭제 또는 뒤에 추가)
		출력
	실행 (여기선 남은 실행 시간 줄이는 것뿐)
	시간할당량 카운트 증가

AT / Arrival TIme : 도착시간
BT / Burst Time : 실행시간
CT / Completion Time : 종료시간
TAT / Turnaround Time : 반환시간
WT / Wait Time : 대기시간
*/




int at[5]; //도착시간
int bt[5]; //실행시간
int tt[5]; //반환시간
int wt[5]; //대기시간
int ct[5]; //종료시간
int q[5] = { 6, 6, 6, 6, 6 }; //준비 큐, 계속 업데이트되며 빈 곳은 6으로 표시
int clen = 0; //큐에 있는 프로세스 개수 (current length)

void qupdate() //큐 값들을 하나씩 앞쪽으로 옮기고 마지막 칸은 비움 - 프로세스 도착이나 time run out 후 추가는 별도
{
	for (int i = 0; i < 4; i++)
	{
		q[i] = q[i + 1];
	}
	clen--;
	q[4] = 6;
}
void qappend(int num) //준비 큐에서 가능한 한 가장 앞쪽에 값을 추가하는 함수 - 길이 계산을 하지 않아도 됨
{
	for (int i = 0; i < 5; i++)
	{
		if (q[i] == 6)
		{
			q[i] = num;
			break;
		}
	}
	clen++;
}
void printSt(int t) //현 상태를 출력하는 함수
{
	int queueV; //빈 queue 출력 시 0 출력 목적
	printf("\n\nTime Unit:%d\tAT\tBT\tCT\tTAT\tWT\n", t);
	for(int i = 0;i<5;i++)
		printf("Process%d:\t%d\t%d\t%d\t%d\t%d\n", i+1, at[i], bt[i], ct[i], tt[i], wt[i]);
	printf("Queue: ");
	for (int i = 0; i < 5; i++)
	{
		if (q[i] == 6)
			queueV = 0;
		else
			queueV = q[i]+1;
		printf("%d ", queueV);
	}
}

void main()
{
	int isarrived = 0; //프로세스 모두 도착 여부
	int cnt = 0; //카운트 (시간 할당량)
	int isalldone = 0; //프로세스 모두 완료 여부
	int isdone[5] = { 0, 0, 0, 0, 0 }; //프로세스 완료 여부
	int arrived[5] = { 0, 0, 0, 0, 0 }; //프로세스 도착 여부
	int tbt[5]; //남은 실행시간
	int tq; //시간 할당량
	int temp;//지금까지 실행한 process - 종료되거나 시간할당량이 끝났는지 확인 //이후 합 저장용으로 사용
	double avg_tat; //평균 반환시간
	double avg_wt; //평균 대기시간

	for (int i = 0; i < 5; i++)
	{
		printf("Process %d : Arrival Time and Burst Time : ", i+1);
		scanf("%d %d", &at[i], &bt[i]);
	}
	printf("\nTime Quanum : ");
	scanf("%d", &tq);

	//임시 bt 를 초깃값으로 설정
	for (int i = 0; i < 5; i++)
	{
		tbt[i] = bt[i];
	}


	for (int t = 0; !isalldone; t++)
	{
		if (!isarrived) //도착 안한 게 있으면
		{
			for (int i = 0; i < 5; i++)
			{
				if (at[i] == t) //모두 검사해서 시간 되면 queue 추가
				{
					qappend(i);
					arrived[i] = 1;
				}
			}
			isarrived = 1;
			for (int i = 0; i < 5; i++)
			{
				if (arrived[i] == 0) //다 들어왔으면 1로 설정
				{
					isarrived = 0;
					break;
				}
			}
		}

		if (t == 0)
			printSt(t); //최초는 큐 업데이트가 없으나 출력

		isalldone = 1;
		for (int i = 0; i < 5; i++)
		{
			if (isdone[i] == 0) //모두 종료되면 반복 탈출
			{
				isalldone = 0;
				break;
			}
		}

		temp = q[0];
		if (temp != 6) //큐에 뭔가 있으면
		{
			if (tbt[temp] == 0) //이전 프로세스 종료시
			{
				ct[temp] = t; //종료시간 기록
				isdone[temp] = 1;
				tt[temp] = ct[temp] - at[temp];
				wt[temp] = tt[temp] - bt[temp];
				qupdate();
				cnt = 0;
				printSt(t);
			}
			else if (cnt == tq) //time quantum 초과 시
			{
				qupdate();
				qappend(temp); //뒤에 추가
				cnt = 0;
				printSt(t);
			}
			
			temp = q[0];
			tbt[temp]--; //실행
			cnt++; //카운트
		}
	}
	temp = 0;
	for (int i = 0; i < 5; i++) //평균 대기시간,반환시간 구하고 출력
		temp += tt[i];
	avg_tat = (float)temp / 5.0;
	temp = 0;
	for (int i = 0; i < 5; i++)
		temp += wt[i];
	avg_wt = (float)temp / 5.0;
	printf("\n\n__________________________________________________\n");
	printSt(0);
	printf("\nAverage Turnaround Time : %lf\nAverage Wait Time : %lf\n\n\n", avg_tat, avg_wt);
	
}