# UE_ShootRPG

### 개발 툴: 언리얼 엔진 (ver. 5.1)
### 개발기간: 2023/07/01 ~
### README최초 작성일: 2023/10/11 

## 업데이트 정보   

###2024/12/08   
크로스 헤어가 제대로 동작하지 않는 현상 수정.   
CharacterStateTypes 세분화.   

###2024/12/07   
무기구조 변경으로 인한 인벤토리 슬롯 fix.   

###2024/11/21   
무기구조 변경.   
기존-> weapon이 pIckupItem을 상속.   
변경-> 독립된 weapon클래스 추가.   

WeaponManager의 추가로 난잡했던 무기 픽업 로직 변경.   

###2024/10/25   
적 리스폰 구현   

###2024/7/15   
버그 픽스 및 닷지 무적 간단 구현   

###2024/07/09   
[몬스터 Cannon & Gunner 디테일 추가(영상)](https://youtu.be/GvwdrbI7uVQ).   
[테스트 플레이(영상)](https://youtu.be/F-Rz4HYntBU).   

### 2024/06/25   
[몬스터 Gunner 추가(영상)](https://youtu.be/KbMwz-B4yj0).   
[몬스터 Cannon추가(영상)](https://youtu.be/KsBltQEAws0).   


### 2024/06/13   
[몬스터 콤보와 무빙(영상)](https://youtu.be/nbt6eRGjTrw).   
한 자리에서 계속해서 공격하지 않고 무빙하며 몬스터들의 배치가 자연스럽게 퍼지게 구현   


### 2024/06/04   
[몬스터 Side Step 구현(영상)](https://youtu.be/Et24etPFmmU?si=OrWUDGnsjEMkv-tN).   
기존에는 몬스터가 몰리면 뒤 늦게 온 몬스터가 앞서 도착한 몬스터에 막혀 공격을 하지 못하는 현상이 있었다.   
해당 현상을 해결하기 위해 앞에 먼저 도착한 몬스터를 확인하고 몬스터가 있으면 side step을 실행시켜 빈 공간으로 유도하도록 했다.   


### 2024/05/24   
[RPG HPBar 구현(영상)](https://youtu.be/A7mOwiHCFLU).   
- #### [그외 커밋 메세지](https://github.com/HundredWed/UE_ShootRPG/commit/e54534c10af5e772968d73662c7155c1ebce965c)

### 2024/05/20   
[데미지 UI 구현(영상)](https://youtu.be/GoyNYIIh_oI).   
기존의 spawn & destory를한 데미지 UI -> 오브젝트풀링으로 변경.     
데미지 UI를 스크린에 그려 매번 Tick을 이용하여 위치를 업데이트 -> widget을 붙인 actor를 생성하는 것으로 변경. 

기타 몬스터 공격 밑작업 


### 2024/05/13
몬스터 클래스 로직 전면 수정.   
[몬스터 AI 기반 구현(영상)](https://youtu.be/WZrKghVmXCo).

버그 이슈    
우클릭 조준 상태에서 닷지시 몽타주 애니메이션 출력이상.    
사격 후 무기 수납 불가.   


### 2024/03/17
Dodge 애니메이션 추가 및 그로인한 캐릭터 상태 변경 [영상]<https://youtu.be/yWqI1j-HFaA>

스노우브레이크 게임의 Dodge조작을 참고

1. 기존: UnEquiped과 Equiped 상태로 나눴지만 상태를 스노우브레이크를 참고하여 UnEquiped,	Equiped, Aim 으로 세분화.   
OrientRotationToMovement의 활성화를 Equiped에서 Aim으로 변경.   

### 2023/10/31

데미지 적용 및 Mover/MoveObject 리펙토링

### 2023/10/22

[몬스터 데미지 엑션 구현(영상)](https://youtu.be/SnpGYMRBWdA).
- #### [겪은 이슈 커밋 메세지](https://github.com/HundredWed/UE_ShootRPG/commit/47deace9ee688919d797d584416da0a967c2df5d)

## 최초 작성일 기준 구현 완료 목록
[영상]<https://youtu.be/NJ5AvRFwZ2c>

### 캐릭터

- 이동 및 점프(기본 상태 / 무기 장착 상태)         
- shift달리기(기본 상태)                 
- 웅크리기(기본 상태 / 무기 장착 상태)              
- 루트 본 회전(기본 상태 / 무기 장착상태)           
- 공격
                                       
### 인벤토리
- 인벤토리 드래그 이벤트
- 슬롯의 드래그 앤 드롭, 우클릭 이벤트
- 월드 버리기
- 분할 기능
- 카테고리 정렬
- (+)배열 조합 기능
- 장착 무기 슬롯

### 월드

- 아이템 월드 상호 작용
- 열쇠 드랍 이벤트로 열리는 문

### 기타 기능

- 각종 무기 어빌리티의 다형성
- 에임 퍼짐
- 탄 퍼짐

### 구현 예정 리스트
- 몬스터
- 보스
- NPC
- 상점
- 스킬
- 게임 저장 및 종료
