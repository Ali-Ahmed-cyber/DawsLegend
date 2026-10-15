#include "DawsMissionSubsystem.h"

namespace
{
    static const TCHAR* MissionIds[] = {
        TEXT("chapter01_radio"), TEXT("chapter01_plaza"), TEXT("chapter01_workshop"), TEXT("chapter01_supplies"), TEXT("chapter01_driver"),
        TEXT("chapter02_market"), TEXT("chapter02_nurse"), TEXT("chapter02_minibus"), TEXT("chapter02_convoy"), TEXT("chapter02_sami"),
        TEXT("chapter03_tawfiq"), TEXT("chapter03_amjad"), TEXT("chapter03_clinic"), TEXT("chapter03_needle"), TEXT("chapter03_bashir"),
        TEXT("chapter04_relay"), TEXT("chapter04_escort"), TEXT("chapter04_broadcast"), TEXT("chapter04_ferry"), TEXT("chapter04_charity"),
        TEXT("chapter05_roadblocks"), TEXT("chapter05_workers"), TEXT("chapter05_tawfiq_debt"), TEXT("chapter05_ledger"), TEXT("chapter05_amjad_truth"),
        TEXT("chapter06_river_road"), TEXT("chapter06_witness_run"), TEXT("chapter06_depot"), TEXT("chapter06_muntasir"), TEXT("chapter06_dawn")
    };
    static const TCHAR* MissionTitles[] = {
        TEXT("A Radio Before Sunrise"), TEXT("Crates With No Seal"), TEXT("Bashir's Back Gate"), TEXT("Three Bundles, One Lie"), TEXT("The Driver Who Ran"),
        TEXT("Smoke Over the Market"), TEXT("The Nurse's Stamp"), TEXT("A Minibus Full of Rumors"), TEXT("The Night Convoy"), TEXT("Sami's Second Chance"),
        TEXT("A Favor for Tawfiq"), TEXT("Amjad's Missing Hours"), TEXT("The Clinic at Dusk"), TEXT("The Needle in the Static"), TEXT("No One Leaves Bashir Behind"),
        TEXT("The Silent Relay"), TEXT("Keep the Technician Moving"), TEXT("A Voice Without a Name"), TEXT("Paper on the Water"), TEXT("The Broker's Charity"),
        TEXT("A City of Roadblocks"), TEXT("Free the Depot Workers"), TEXT("Tawfiq's Debt"), TEXT("The Ledger Has Two Pages"), TEXT("Amjad Tells the Truth"),
        TEXT("Road to the River"), TEXT("The Witness Protection Run"), TEXT("Hold the Central Depot"), TEXT("The Man Behind the Manifest"), TEXT("Dawn of Al-Mirfa")
    };
    static const TCHAR* DialogueText[] = {
        TEXT("هبة في الراديو: يا أهل المرفأ، شحنة علاج اختفت قبل الفجر. قابلوا أصحابكم واتبعوا أثر الختم."),
        TEXT("توفيق: يا زول الكراتين فاضية عديل! الزول البوقّع استلام الهوا ده منو؟"),
        TEXT("بشير: في زول داير يمسح دفتر الورشة. ساعدوني أنقذه وبوريكم طريق خلفي ما معروف."),
        TEXT("أمجد: تلات ربطات وتلاتة أكواد للعيادة، لكن الورق كاتب عربية واحدة. الحكاية ما راكبة."),
        TEXT("بشير: الهايلوكس دي بتاعة سامي. ألحقوه لو قدرتوا، لكن أول حاجة افهموا هو هارب من شنو."),
        TEXT("هبة: الدخان طالع من السوق. فتشوا المخزن المحروق قبل الدليل يضيع."),
        TEXT("سلوى: الختم مضروب. الصناديق اتبدلت علاماتها بعد ما طلعت من المخزن."),
        TEXT("إدريس: أركبوا معاي. كل ست شاي عندها رواية، وواحدة منهن حتطابق خط السير."),
        TEXT("أمجد: اتبعوا آثار الكفرات. المسجل ممكن يورينا منو أصدر أمر مشوار الليل."),
        TEXT("سامي: سقت الشحنة عشان هددوني بأهلي. بقدر أشهد، بس لازم تأمّنوا ليهم الحماية."),
        TEXT("توفيق: لقيت لينا درب مختصر. أهدوا يا جماعة، شنو الممكن يحصل يعني؟"),
        TEXT("أمجد: أنا زمان سلّمت طرد مقفول لمُنتصر. كان مفروض أقول ليكم من بدري."),
        TEXT("سلوى: المولد قرب يطفي. أمّنوا طريق العيادة لحدي ما ننقل العلاج."),
        TEXT("هبة: في زول مشوّش الإشارة. قائمة الترددات ممكن تكشف منو بتنصّت علينا."),
        TEXT("بشير: حاصروني جوة ورشتي. طلّعوني وخلو الدفتر بعيد من الموية والنار."),
        TEXT("الفني: الإشارة قربت ترجع. خلّوا الشارع فاضي لحدي ما أوصل السلك."),
        TEXT("النقيب نادر: ما تخلّوا الفني وراكم. من غير شهادته ملفنا ناقص."),
        TEXT("هبة: حنبث الوقائع المتأكدين منها من غير ما نكشف اسم الشاهد. أمّنوا الاستديو."),
        TEXT("مريم: شفت شنطة تحت مرسى العبّارة. الجماعة الخلوها كانوا مستعجلين شديد."),
        TEXT("الوسيط: نحن البنوفر الأكل للمدينة. غريبة الاتهامات بتجي قبل الدليل!"),
        TEXT("إدريس: الحواجز دي كضب. في زول داير عربية العيادة تتأخر عن موعدها."),
        TEXT("العامل: رفضنا نغيّر عدد الشحنة، قاموا حبسونا جوة الساحة."),
        TEXT("توفيق: هم عرفوا طريقنا بسبب الاختصار. أنا اتسرعت وورّطت الفريق كله."),
        TEXT("بشير: الصفحة اتقطعت بنضافة. لقوا النسخة الكربونية قبل ما يحرقوا السجلات."),
        TEXT("أمجد: ما كنت عارف جوة الطرد شنو. دي خريطة الطريق ودفتر العربية؛ أحكموا عليّ بالعمل البعملو هسي."),
        TEXT("بشير: العربية جاهزة. إدريس حيدلّكم على اللفات الآمنة. وصلوا شحنة العيادة."),
        TEXT("النقيب نادر: خلّوا سامي والشاهد في أمان. ما دايرين بطولة تعرّضهم للخطر."),
        TEXT("فوزي: الحلة كلها جابت موية وشاش. اثبتوا في المخزن واحموا غرفة الأدلة."),
        TEXT("منتصر: الورق هو القوة. من غير الصفحة الناقصة عندكم شكوك ساكت، ما قضية."),
        TEXT("هبة: يا مرفأ، المستندات اتراجعت واتأكدنا منها. المدينة تستاهل الحقيقة؛ رجّعوا العمال لأهلهم أولاً.")
    };
    static const TCHAR* TaskTypes[] = {
        TEXT("talk"), TEXT("reach"), TEXT("talk"), TEXT("collect"), TEXT("defeat"),
        TEXT("evidence"), TEXT("talk"), TEXT("talk"), TEXT("evidence"), TEXT("talk"),
        TEXT("reach"), TEXT("talk"), TEXT("defeat"), TEXT("evidence"), TEXT("talk"),
        TEXT("talk"), TEXT("reach"), TEXT("talk"), TEXT("evidence"), TEXT("talk"),
        TEXT("reach"), TEXT("defeat"), TEXT("talk"), TEXT("evidence"), TEXT("talk"),
        TEXT("defeat"), TEXT("reach"), TEXT("defeat"), TEXT("talk"), TEXT("talk")
    };
    static const TCHAR* TaskTargets[] = {
        TEXT("hiba"), TEXT(""), TEXT("bashir"), TEXT(""), TEXT(""),
        TEXT(""), TEXT("salwa"), TEXT("idris"), TEXT(""), TEXT("sami"),
        TEXT(""), TEXT("amjad"), TEXT(""), TEXT(""), TEXT("bashir"),
        TEXT("technician"), TEXT(""), TEXT("hiba"), TEXT(""), TEXT("the_broker"),
        TEXT(""), TEXT(""), TEXT("tawfiq"), TEXT(""), TEXT("amjad"),
        TEXT(""), TEXT(""), TEXT(""), TEXT("muntasir"), TEXT("the_broker")
    };
    static const int32 RequiredCounts[] = {
        1,0,1,3,2, 1,1,1,1,1, 0,1,3,1,1, 1,0,1,1,1, 0,4,1,1,1, 4,0,5,1,1
    };
    static const TCHAR* ObjectivesText[] = {
        TEXT("Meet Tawfiq and Amjad, then listen to Hiba's report"), TEXT("Secure the plaza and inspect the empty crates"),
        TEXT("Reach Bashir's workshop and save his ledger"), TEXT("Recover three supply bundles and compare their tags"),
        TEXT("Intercept the fleeing driver and recover his delivery token"),
        TEXT("Search the burned market store"), TEXT("Show Salwa the counterfeit medical seal"),
        TEXT("Speak to Idris and confirm the river-road route"), TEXT("Recover the night convoy recorder"),
        TEXT("Speak to Sami and get him safely to Captain Nadir"),
        TEXT("Reach the pickup and regroup after the shortcut"), TEXT("Speak to Amjad about the missing hours"),
        TEXT("Protect the clinic supply route"), TEXT("Recover the coded radio frequency list"),
        TEXT("Reach Bashir and rescue him from the workshop"),
        TEXT("Speak to the Technician and restore the relay"), TEXT("Escort the Technician to the broadcast room"),
        TEXT("Speak to Hiba and secure the verified broadcast"), TEXT("Recover the waterproof case at the ferry landing"),
        TEXT("Reach the charity event and identify Muntasir's handoff"),
        TEXT("Reopen the safe route through the false roadblocks"), TEXT("Defeat the guards and free the depot workers"),
        TEXT("Find Tawfiq and bring him back safely"), TEXT("Recover the missing ledger page"),
        TEXT("Speak to Amjad and secure the vehicle logbook"), TEXT("Clear the river road for the clinic truck"),
        TEXT("Move Sami and the Survivor to the safehouse"), TEXT("Defend the central depot and its evidence room"),
        TEXT("Confront Muntasir and recover the missing ledger page"), TEXT("Confront the Broker and complete the final broadcast")
    };
}

void UDawsMissionSubsystem::StartMission(FString MissionId, FText MissionTitle, const TArray<FDawsMissionObjective>& Objectives)
{
    CurrentMissionId = MoveTemp(MissionId);
    CurrentMissionTitle = MoveTemp(MissionTitle);
    CurrentObjectives = Objectives;
}
void UDawsMissionSubsystem::StartCampaign()
{
    CampaignMissionIndex = 0;
    bCampaignActive = true;
    StartCampaignMission(CampaignMissionIndex);
}
bool UDawsMissionSubsystem::RestoreCampaignAtMission(const FString& MissionId)
{
    if (MissionId == TEXT("campaign_complete")) {
        bCampaignActive = false; CampaignMissionIndex = 30; CurrentMissionId = MissionId;
        CurrentMissionTitle = FText::FromString(TEXT("Dawn of Al-Mirfa — Campaign Complete"));
        CurrentObjectives.Reset(); return true;
    }
    for (int32 Index = 0; Index < UE_ARRAY_COUNT(MissionIds); ++Index) {
        if (MissionId == MissionIds[Index]) { bCampaignActive = true; StartCampaignMission(Index); return true; }
    }
    return false;
}
void UDawsMissionSubsystem::StartCampaignMission(int32 MissionIndex)
{
    if (MissionIndex < 0 || MissionIndex >= UE_ARRAY_COUNT(MissionIds)) {
        bCampaignActive = false; CampaignMissionIndex = 30;
        CurrentMissionId = TEXT("campaign_complete");
        CurrentMissionTitle = FText::FromString(TEXT("Dawn of Al-Mirfa — Campaign Complete"));
        CurrentObjectives.Reset(); CurrentTaskType = TEXT("none"); CurrentTaskTargetId.Reset();
        CurrentTaskProgress = 0; RequiredTaskProgress = 0; return;
    }
    CampaignMissionIndex = MissionIndex;
    CurrentDialogue = FText::FromString(DialogueText[MissionIndex]);
    CurrentTaskType = TaskTypes[MissionIndex];
    CurrentTaskTargetId = TaskTargets[MissionIndex];
    CurrentTaskProgress = 0;
    RequiredTaskProgress = RequiredCounts[MissionIndex];
    FDawsMissionObjective Objective;
    Objective.Id = FString(MissionIds[MissionIndex]) + TEXT("_reach");
    Objective.Description = FText::FromString(ObjectivesText[MissionIndex]);
    Objective.bCompleted = false;
    TArray<FDawsMissionObjective> Objectives; Objectives.Add(Objective);
    StartMission(MissionIds[MissionIndex], FText::FromString(MissionTitles[MissionIndex]), Objectives);
}
void UDawsMissionSubsystem::RegisterTaskProgress(const FString& TaskType, int32 Amount)
{
    if (TaskType != CurrentTaskType || Amount <= 0 || RequiredTaskProgress <= 0) return;
    CurrentTaskProgress = FMath::Clamp(CurrentTaskProgress + Amount, 0, RequiredTaskProgress);
}
void UDawsMissionSubsystem::RestoreTaskProgress(int32 Progress)
{
    CurrentTaskProgress = FMath::Clamp(Progress, 0, RequiredTaskProgress);
}
void UDawsMissionSubsystem::RegisterNPCInteraction(const FString& NPCId)
{
    if (CurrentTaskType == TEXT("talk") && CurrentTaskTargetId == NPCId) RegisterTaskProgress(TEXT("talk"));
}
bool UDawsMissionSubsystem::CanCompleteCurrentMission() const
{
    return RequiredTaskProgress <= 0 || CurrentTaskProgress >= RequiredTaskProgress;
}
bool UDawsMissionSubsystem::CompleteObjective(FString ObjectiveId)
{
    for (FDawsMissionObjective& Objective : CurrentObjectives) {
        if (Objective.Id == ObjectiveId && !Objective.bCompleted) {
            Objective.bCompleted = true;
            if (bCampaignActive && IsMissionComplete()) StartCampaignMission(CampaignMissionIndex + 1);
            return true;
        }
    }
    return false;
}
void UDawsMissionSubsystem::ResetCurrentMission()
{
    for (FDawsMissionObjective& Objective : CurrentObjectives) Objective.bCompleted = false;
}
bool UDawsMissionSubsystem::IsMissionComplete() const
{
    if (CurrentObjectives.IsEmpty()) return false;
    for (const FDawsMissionObjective& Objective : CurrentObjectives) if (!Objective.bCompleted) return false;
    return true;
}
