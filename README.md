# UE_ShootRPG

### 개발 툴: 언리얼 엔진 (ver. 5.1)
### 개발기간: 2023/07/01 ~
### README최초 작성일: 2023/10/11 

## 업데이트 정보

2024/03/17
Dodge 애니메이션 추가 및 그로인한 캐릭터 상태 변경 [영상]<https://youtu.be/yWqI1j-HFaA>

스노우브레이크 게임의 Dodge조작을 참고

1. 기존: UnEquiped과 Equiped 상태로 나눴지만 상태를 스노우브레이크를 참고하여 UnEquiped,	Equiped, Aim 으로 세분화.
OrientRotationToMovement의 활성화를 Equiped에서 Aim으로 변경.

2023/10/31

데미지 적용 및 Mover/MoveObject 리펙토링

2023/10/22

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
