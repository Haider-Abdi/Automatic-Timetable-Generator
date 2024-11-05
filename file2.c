#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SECTIONS 5
#define DAYS_PER_WEEK 5
#define SLOTS_PER_DAY 8
#define BREAK_SLOT 4
#define MAX_SUBJECTS 15
#define MAX_NAME 10
#define MAX_ATTEMPTS 100 // Increased from 50
#define MAX_FACULTY 6

#define MAX_LAB_SLOTS_PER_DAY 2
#define MAX_LECTURE_SLOTS_PER_DAY 1
#define MAX_TUT_SLOTS_PER_DAY 1
#define MAX_TOTAL_SLOTS_PER_DAY 2

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct Subject;
struct Faculty;

typedef enum
{
    PATTERN_4_LECTURES,
    PATTERN_3_1_2,
    PATTERN_2_2,
    PATTERN_2_LECTURES
} CoursePattern;

typedef struct
{
    char faculty_initials[4];
    struct Subject *subject;
} SectionFacultyAssignment;

typedef struct
{
    char initials[4];
    bool used;
} UniqueFaculty;

typedef struct
{
    char initials[4];
    bool is_available[DAYS_PER_WEEK][SLOTS_PER_DAY];
} Faculty;

typedef struct
{
    char name[MAX_NAME];
    CoursePattern pattern;
    Faculty faculty[MAX_FACULTY];
    int num_faculty;
} Subject;

typedef struct
{
    char faculty_initials[4];
    int section;
    char subject[MAX_NAME];
    bool is_lab;
    bool is_tutorial;
} FacultyAssignment;

FacultyAssignment faculty_schedule[DAYS_PER_WEEK][SLOTS_PER_DAY][MAX_SECTIONS];

typedef struct
{
    char name[MAX_NAME];
    bool is_lab;
    bool is_tutorial;
    char faculty_initials[4];
} TimeSlot;

TimeSlot timetable[MAX_SECTIONS][DAYS_PER_WEEK][SLOTS_PER_DAY];
Subject subjects[MAX_SUBJECTS];

TimeSlot timetable[MAX_SECTIONS][DAYS_PER_WEEK][SLOTS_PER_DAY];
Subject subjects[MAX_SUBJECTS];
int num_subjects = 0;

SectionFacultyAssignment section_faculty[MAX_SECTIONS][MAX_SUBJECTS];
FacultyAssignment faculty_schedule[DAYS_PER_WEEK][SLOTS_PER_DAY][MAX_SECTIONS];
TimeSlot timetable[MAX_SECTIONS][DAYS_PER_WEEK][SLOTS_PER_DAY];
Subject subjects[MAX_SUBJECTS];

void initializeFacultySchedule()
{
    for (int day = 0; day < DAYS_PER_WEEK; day++)
    {
        for (int slot = 0; slot < SLOTS_PER_DAY; slot++)
        {
            for (int sec = 0; sec < MAX_SECTIONS; sec++)
            {
                memset(&faculty_schedule[day][slot][sec], 0, sizeof(FacultyAssignment));
            }
        }
    }
}

bool isFacultyAvailable(const char *faculty_initials, int day, int slot)
{
    for (int sec = 0; sec < MAX_SECTIONS; sec++)
    {
        if (strlen(faculty_schedule[day][slot][sec].faculty_initials) > 0 &&
            strcmp(faculty_schedule[day][slot][sec].faculty_initials, faculty_initials) == 0)
        {
            return false; // Faculty is already assigned in this slot
        }
    }
    return true;
}

bool isSlotFree(int section, int day, int slot)
{
    return strlen(timetable[section][day][slot].name) == 0;
}

void resetFacultyAvailability()
{
    for (int i = 0; i < MAX_SUBJECTS; i++)
    {
        for (int f = 0; f < subjects[i].num_faculty; f++)
        {
            for (int day = 0; day < DAYS_PER_WEEK; day++)
            {
                for (int slot = 0; slot < SLOTS_PER_DAY; slot++)
                {
                    subjects[i].faculty[f].is_available[day][slot] = true;
                }
            }
        }
    }
}

bool isEndOfWeekSlot(int day, int slot)
{
    return (day == DAYS_PER_WEEK - 1 && slot >= SLOTS_PER_DAY - 3);
}

int countEndOfWeekEmptySlots(int section)
{
    int empty_count = 0;
    int last_day = DAYS_PER_WEEK - 1;

    for (int slot = SLOTS_PER_DAY - 3; slot < SLOTS_PER_DAY; slot++)
    {
        if (isSlotFree(section, last_day, slot) && slot != BREAK_SLOT)
        {
            empty_count++;
        }
    }
    return empty_count;
}

int getUniqueFaculties(UniqueFaculty *unique_faculties)
{
    int count = 0;

    for (int i = 0; i < MAX_SUBJECTS * MAX_FACULTY; i++)
    {
        unique_faculties[i].used = false;
    }

    // Collect unique faculty initials from all subjects
    for (int i = 0; i < num_subjects; i++)
    {
        for (int j = 0; j < subjects[i].num_faculty; j++)
        {
            bool found = false;
            // Check if faculty already exists in unique list
            for (int k = 0; k < count; k++)
            {
                if (strcmp(unique_faculties[k].initials, subjects[i].faculty[j].initials) == 0)
                {
                    found = true;
                    break;
                }
            }
            // Add new faculty if not found
            if (!found)
            {
                strcpy(unique_faculties[count].initials, subjects[i].faculty[j].initials);
                unique_faculties[count].used = true;
                count++;
            }
        }
    }
    return count;
}

void initializeSubjects()
{
    // DSA: 3 lectures + 1 tutorial + 2 lab
    strcpy(subjects[num_subjects].name, "DSA");
    subjects[num_subjects].pattern = PATTERN_3_1_2;
    subjects[num_subjects].num_faculty = 4;  // Reduced from 6
    strcpy(subjects[num_subjects].faculty[0].initials, "SKG");
    strcpy(subjects[num_subjects].faculty[1].initials, "RVK");
    strcpy(subjects[num_subjects].faculty[2].initials, "AKS");
    strcpy(subjects[num_subjects].faculty[3].initials, "PKS");
    num_subjects++;

    // OS: 2 lectures + 2 lab
    strcpy(subjects[num_subjects].name, "OS");
    subjects[num_subjects].pattern = PATTERN_2_2;
    subjects[num_subjects].num_faculty = 4;  // Reduced from 6
    strcpy(subjects[num_subjects].faculty[0].initials, "PKM");
    strcpy(subjects[num_subjects].faculty[1].initials, "NVR");
    strcpy(subjects[num_subjects].faculty[2].initials, "RSH");
    strcpy(subjects[num_subjects].faculty[3].initials, "AKR");
    num_subjects++;

    // DBMS: 4 lectures
    strcpy(subjects[num_subjects].name, "DBMS");
    subjects[num_subjects].pattern = PATTERN_4_LECTURES;
    subjects[num_subjects].num_faculty = 4;  // Reduced from 6
    strcpy(subjects[num_subjects].faculty[0].initials, "SVK");
    strcpy(subjects[num_subjects].faculty[1].initials, "PKR");
    strcpy(subjects[num_subjects].faculty[2].initials, "MTH");
    strcpy(subjects[num_subjects].faculty[3].initials, "RKS");
    num_subjects++;

    // Ethics: 2 lectures
    strcpy(subjects[num_subjects].name, "Ethics");
    subjects[num_subjects].pattern = PATTERN_2_LECTURES;
    subjects[num_subjects].num_faculty = 3;  // Reduced from 6
    strcpy(subjects[num_subjects].faculty[0].initials, "SKN");
    strcpy(subjects[num_subjects].faculty[1].initials, "RKM");
    strcpy(subjects[num_subjects].faculty[2].initials, "PKV");
    num_subjects++;

    // COA: 3 lectures + 1 tutorial + 2 lab
    strcpy(subjects[num_subjects].name, "COA");
    subjects[num_subjects].pattern = PATTERN_3_1_2;
    subjects[num_subjects].num_faculty = 4;  // Reduced from 6
    strcpy(subjects[num_subjects].faculty[0].initials, "RMS");
    strcpy(subjects[num_subjects].faculty[1].initials, "SKP");
    strcpy(subjects[num_subjects].faculty[2].initials, "VKR");
    strcpy(subjects[num_subjects].faculty[3].initials, "NSR");
    num_subjects++;

    // OOPS: 2 lectures + 2 lab
    strcpy(subjects[num_subjects].name, "OOPS");
    subjects[num_subjects].pattern = PATTERN_2_2;
    subjects[num_subjects].num_faculty = 4;  // Reduced from 6
    strcpy(subjects[num_subjects].faculty[0].initials, "KMS");
    strcpy(subjects[num_subjects].faculty[1].initials, "PRS");
    strcpy(subjects[num_subjects].faculty[2].initials, "ANS");
    strcpy(subjects[num_subjects].faculty[3].initials, "MKS");
    num_subjects++;

    // Math: 4 lectures
    strcpy(subjects[num_subjects].name, "Math");
    subjects[num_subjects].pattern = PATTERN_4_LECTURES;
    subjects[num_subjects].num_faculty = 4;  // Reduced from 6
    strcpy(subjects[num_subjects].faculty[0].initials, "RPK");
    strcpy(subjects[num_subjects].faculty[1].initials, "SNK");
    strcpy(subjects[num_subjects].faculty[2].initials, "VMK");
    strcpy(subjects[num_subjects].faculty[3].initials, "KNS");
    num_subjects++;

    resetFacultyAvailability();
}

bool checkFacultyWorkload(const char *faculty_initials)
{
    int total_slots = 0;
    for (int day = 0; day < DAYS_PER_WEEK; day++)
    {
        for (int slot = 0; slot < SLOTS_PER_DAY; slot++)
        {
            for (int sec = 0; sec < MAX_SECTIONS; sec++)
            {
                if (strcmp(faculty_schedule[day][slot][sec].faculty_initials, faculty_initials) == 0)
                {
                    total_slots++;
                }
            }
        }
    }
    return total_slots < 20; // Maximum 20 slots per faculty per week
}

bool areConsecutiveSlotsFree(int section, int day, int start_slot, int count)
{
    if (start_slot + count > SLOTS_PER_DAY)
        return false;
    for (int i = 0; i < count; i++)
    {
        if (!isSlotFree(section, day, start_slot + i))
        {
            return false;
        }
    }
    return true;
}

bool isWellDistributed(int section, int day, int slot, const char *subject_name)
{
    // Count lectures on this day
    int lectures_this_day = 0;
    for (int s = 0; s < SLOTS_PER_DAY; s++)
    {
        if (strcmp(timetable[section][day][s].name, subject_name) == 0)
        {
            lectures_this_day++;
        }
    }

    // Check if there are too many lectures in adjacent slots
    int adjacent_lectures = 0;
    for (int s = MAX(0, slot - 1); s <= MIN(SLOTS_PER_DAY - 1, slot + 1); s++)
    {
        if (s != BREAK_SLOT && strlen(timetable[section][day][s].name) > 0)
        {
            adjacent_lectures++;
        }
    }

    // Count lectures in the same time slot on other days
    int lectures_same_slot = 0;
    for (int d = 0; d < DAYS_PER_WEEK; d++)
    {
        if (strlen(timetable[section][d][slot].name) > 0)
        {
            lectures_same_slot++;
        }
    }

    // Calculate empty end-of-week slots
    int empty_end_slots = countEndOfWeekEmptySlots(section);

    // Prioritize end-of-week slots if they're empty
    if (isEndOfWeekSlot(day, slot) && empty_end_slots > 1)
    {
        return true; // Strongly prefer filling end-of-week slots
    }

    // Regular distribution checks
    return lectures_this_day <= 2 &&
           adjacent_lectures <= 1 &&
           lectures_same_slot <= 2;
}

bool checkDailySlotLimits(int section, int day, const char *subject_name, bool is_lab, bool is_tutorial)
{
    int total_slots = 0;
    int lab_count = 0;
    int lecture_count = 0;
    int tutorial_count = 0;

    // Count existing slots
    for (int slot = 0; slot < SLOTS_PER_DAY; slot++)
    {
        if (strcmp(timetable[section][day][slot].name, subject_name) == 0)
        {
            total_slots++;
            if (timetable[section][day][slot].is_lab)
                lab_count++;
            else if (timetable[section][day][slot].is_tutorial)
                tutorial_count++;
            else
                lecture_count++;
        }
    }

    // Check if adding new slot would exceed limits
    if (total_slots >= MAX_TOTAL_SLOTS_PER_DAY)
        return false;
    if (is_lab && (lab_count >= MAX_LAB_SLOTS_PER_DAY))
        return false;
    if (is_tutorial && (tutorial_count >= MAX_TUT_SLOTS_PER_DAY))
        return false;
    if (!is_lab && !is_tutorial && (lecture_count >= MAX_LECTURE_SLOTS_PER_DAY))
        return false;

    return true;
}

void updateFacultySchedule(int section, int day, int slot, const char *faculty_initials,
                           const char *subject, bool is_lab, bool is_tutorial)
{
    strcpy(faculty_schedule[day][slot][section].faculty_initials, faculty_initials);
    strcpy(faculty_schedule[day][slot][section].subject, subject);
    faculty_schedule[day][slot][section].section = section;
    faculty_schedule[day][slot][section].is_lab = is_lab;
    faculty_schedule[day][slot][section].is_tutorial = is_tutorial;
}

bool assignSlot(int section, int day, int slot, Subject *subject, Faculty *faculty,
                bool is_lab, bool is_tutorial)
{
    // First check if faculty is available
    if (!isFacultyAvailable(faculty->initials, day, slot))
    {
        return false;
    }

    // If available, proceed with assignment
    strcpy(timetable[section][day][slot].name, subject->name);
    timetable[section][day][slot].is_lab = is_lab;
    timetable[section][day][slot].is_tutorial = is_tutorial;
    strcpy(timetable[section][day][slot].faculty_initials, faculty->initials);

    // Update faculty schedule
    updateFacultySchedule(section, day, slot, faculty->initials,
                          subject->name, is_lab, is_tutorial);

    return true;
}

bool findFreeSlot(int section, int *day, int *slot, Subject *subject, bool is_lab, bool is_tutorial)
{
    // First check end-of-week slots if they're too empty
    if (countEndOfWeekEmptySlots(section) > 1)
    {
        int last_day = DAYS_PER_WEEK - 1;
        for (int s = SLOTS_PER_DAY - 3; s < SLOTS_PER_DAY; s++)
        {
            if (s != BREAK_SLOT && isSlotFree(section, last_day, s) &&
                checkDailySlotLimits(section, last_day, subject->name, is_lab, is_tutorial))
            {
                *day = last_day;
                *slot = s;
                return true;
            }
        }
    }

    // Try to find a well-distributed slot using round-robin approach
    for (int attempt = 0; attempt < DAYS_PER_WEEK; attempt++)
    {
        // Start from a different day each time to avoid clustering
        int start_day = rand() % DAYS_PER_WEEK;

        for (int d_offset = 0; d_offset < DAYS_PER_WEEK; d_offset++)
        {
            int d = (start_day + d_offset) % DAYS_PER_WEEK;

            // Alternate between morning and afternoon slots
            for (int s_group = 0; s_group < 2; s_group++)
            {
                int start_slot = s_group == 0 ? 0 : BREAK_SLOT + 1;
                int end_slot = s_group == 0 ? BREAK_SLOT : SLOTS_PER_DAY;

                for (int s = start_slot; s < end_slot; s++)
                {
                    if (s != BREAK_SLOT && isSlotFree(section, d, s) &&
                        checkDailySlotLimits(section, d, subject->name, is_lab, is_tutorial) &&
                        isWellDistributed(section, d, s, subject->name))
                    {
                        *day = d;
                        *slot = s;
                        return true;
                    }
                }
            }
        }
    }

    // Fall back to any available slot if no well-distributed slot is found
    for (int d = 0; d < DAYS_PER_WEEK; d++)
    {
        for (int s = 0; s < SLOTS_PER_DAY; s++)
        {
            if (s != BREAK_SLOT && isSlotFree(section, d, s) &&
                checkDailySlotLimits(section, d, subject->name, is_lab, is_tutorial))
            {
                *day = d;
                *slot = s;
                return true;
            }
        }
    }
    return false;
}

bool findFreeLab(int section, int *day, int *slot, Subject *subject)
{
    // First try to find well-distributed lab slots
    for (int d = 0; d < DAYS_PER_WEEK; d++)
    {
        for (int s = 0; s < BREAK_SLOT - 1; s++)
        {
            if (areConsecutiveSlotsFree(section, d, s, 2) &&
                checkDailySlotLimits(section, d, subject->name, true, false) &&
                isWellDistributed(section, d, s, subject->name))
            {
                *day = d;
                *slot = s;
                return true;
            }
        }
    }

    // Fall back to any available consecutive slots
    for (int d = 0; d < DAYS_PER_WEEK; d++)
    {
        for (int s = 0; s < BREAK_SLOT - 1; s++)
        {
            if (areConsecutiveSlotsFree(section, d, s, 2) &&
                checkDailySlotLimits(section, d, subject->name, true, false))
            {
                *day = d;
                *slot = s;
                return true;
            }
        }
    }
    return false;
}

void shuffleSubjects(int *order, int count)
{
    for (int i = count - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }
}

void assignFacultyToSections() {
    for (int sec = 0; sec < MAX_SECTIONS; sec++) {
        for (int sub = 0; sub < num_subjects; sub++) {
            // Try each faculty member until finding one with acceptable workload
            bool assigned = false;
            for (int attempts = 0; attempts < subjects[sub].num_faculty * 2 && !assigned; attempts++) {
                int faculty_index = rand() % subjects[sub].num_faculty;
                if (checkFacultyWorkload(subjects[sub].faculty[faculty_index].initials)) {
                    strcpy(section_faculty[sec][sub].faculty_initials, 
                           subjects[sub].faculty[faculty_index].initials);
                    section_faculty[sec][sub].subject = &subjects[sub];
                    assigned = true;
                }
            }
            // If no faculty with good workload found, assign anyway
            if (!assigned) {
                int faculty_index = rand() % subjects[sub].num_faculty;
                strcpy(section_faculty[sec][sub].faculty_initials, 
                       subjects[sub].faculty[faculty_index].initials);
                section_faculty[sec][sub].subject = &subjects[sub];
            }
        }
    }
}

bool schedulePattern(int section, Subject *subject)
{
    // Find the pre-assigned faculty for this section and subject
    Faculty *assigned_faculty = NULL;
    for (int i = 0; i < num_subjects; i++)
    {
        if (section_faculty[section][i].subject == subject)
        {
            // Find the corresponding faculty object
            for (int f = 0; f < subject->num_faculty; f++)
            {
                if (strcmp(subject->faculty[f].initials,
                           section_faculty[section][i].faculty_initials) == 0)
                {
                    assigned_faculty = &subject->faculty[f];
                    break;
                }
            }
            break;
        }
    }

    if (!assigned_faculty)
        return false;

    int attempts = 0;

    switch (subject->pattern)
    {
    case PATTERN_4_LECTURES:
    {
        int scheduled = 0;
        while (scheduled < 4 && attempts < MAX_ATTEMPTS)
        {
            int day, slot;
            if (findFreeSlot(section, &day, &slot, subject, false, false))
            {
                if (isFacultyAvailable(assigned_faculty->initials, day, slot))
                {
                    if (assignSlot(section, day, slot, subject, assigned_faculty, false, false))
                    {
                        scheduled++;
                    }
                }
            }
            attempts++;
        }
        return scheduled == 4;
    }

    case PATTERN_3_1_2:
    {
        // Schedule lab slots
        attempts = 0;
        bool lab_scheduled = false;
        while (!lab_scheduled && attempts < MAX_ATTEMPTS)
        {
            int day, slot;
            if (findFreeLab(section, &day, &slot, subject))
            {
                if (isFacultyAvailable(assigned_faculty->initials, day, slot) &&
                    isFacultyAvailable(assigned_faculty->initials, day, slot + 1))
                {
                    if (assignSlot(section, day, slot, subject, assigned_faculty, true, false) &&
                        assignSlot(section, day, slot + 1, subject, assigned_faculty, true, false))
                    {
                        lab_scheduled = true;
                    }
                }
            }
            attempts++;
        }
        if (!lab_scheduled)
            return false;

        // Schedule lectures and tutorial
        int lectures_scheduled = 0;
        bool tutorial_scheduled = false;
        attempts = 0;

        while ((lectures_scheduled < 3 || !tutorial_scheduled) && attempts < MAX_ATTEMPTS)
        {
            int day, slot;
            if (findFreeSlot(section, &day, &slot, subject, false, !tutorial_scheduled))
            {
                if (isFacultyAvailable(assigned_faculty->initials, day, slot))
                {
                    if (!tutorial_scheduled)
                    {
                        if (assignSlot(section, day, slot, subject, assigned_faculty, false, true))
                        {
                            tutorial_scheduled = true;
                        }
                    }
                    else if (lectures_scheduled < 3)
                    {
                        if (assignSlot(section, day, slot, subject, assigned_faculty, false, false))
                        {
                            lectures_scheduled++;
                        }
                    }
                }
            }
            attempts++;
        }
        return lectures_scheduled == 3 && tutorial_scheduled;
    }

    case PATTERN_2_2:
    {
        // Schedule lab slots
        attempts = 0;
        bool lab_scheduled = false;
        while (!lab_scheduled && attempts < MAX_ATTEMPTS)
        {
            int day, slot;
            if (findFreeLab(section, &day, &slot, subject))
            {
                if (isFacultyAvailable(assigned_faculty->initials, day, slot) &&
                    isFacultyAvailable(assigned_faculty->initials, day, slot + 1))
                {
                    if (assignSlot(section, day, slot, subject, assigned_faculty, true, false) &&
                        assignSlot(section, day, slot + 1, subject, assigned_faculty, true, false))
                    {
                        lab_scheduled = true;
                    }
                }
            }
            attempts++;
        }
        if (!lab_scheduled)
            return false;

        // Schedule lectures
        int lectures_scheduled = 0;
        attempts = 0;
        while (lectures_scheduled < 2 && attempts < MAX_ATTEMPTS)
        {
            int day, slot;
            if (findFreeSlot(section, &day, &slot, subject, false, false))
            {
                if (isFacultyAvailable(assigned_faculty->initials, day, slot))
                {
                    if (assignSlot(section, day, slot, subject, assigned_faculty, false, false))
                    {
                        lectures_scheduled++;
                    }
                }
            }
            attempts++;
        }
        return lectures_scheduled == 2;
    }

    case PATTERN_2_LECTURES:
    {
        int scheduled = 0;
        attempts = 0;
        while (scheduled < 2 && attempts < MAX_ATTEMPTS)
        {
            int day, slot;
            if (findFreeSlot(section, &day, &slot, subject, false, false))
            {
                if (isFacultyAvailable(assigned_faculty->initials, day, slot))
                {
                    if (assignSlot(section, day, slot, subject, assigned_faculty, false, false))
                    {
                        scheduled++;
                    }
                }
            }
            attempts++;
        }
        return scheduled == 2;
    }
    }
    return false;
}
void generateTimetable() {
    srand(time(NULL));
    initializeSubjects();
    
    bool complete = false;
    int total_attempts = 0;
    
    while (!complete && total_attempts < MAX_ATTEMPTS * 2) {
        initializeFacultySchedule();
        assignFacultyToSections();
        
        // Clear timetable and set breaks
        for (int sec = 0; sec < MAX_SECTIONS; sec++) {
            for (int day = 0; day < DAYS_PER_WEEK; day++) {
                for (int slot = 0; slot < SLOTS_PER_DAY; slot++) {
                    memset(&timetable[sec][day][slot], 0, sizeof(TimeSlot));
                    if (slot == BREAK_SLOT) {
                        strcpy(timetable[sec][day][slot].name, "BREAK");
                    }
                }
            }
        }

        bool all_sections_complete = true;
        
        // Schedule each section
        for (int sec = 0; sec < MAX_SECTIONS; sec++) {
            printf("Scheduling section %d...\n", sec + 1);
            bool section_complete = false;
            int section_attempts = 0;
            
            while (!section_complete && section_attempts < MAX_ATTEMPTS) {
                // Reset timetable for this section
                for (int day = 0; day < DAYS_PER_WEEK; day++) {
                    for (int slot = 0; slot < SLOTS_PER_DAY; slot++) {
                        if (slot != BREAK_SLOT) {
                            memset(&timetable[sec][day][slot], 0, sizeof(TimeSlot));
                        }
                    }
                }
                
                // Create array for randomized subject order
                int subject_order[MAX_SUBJECTS];
                for (int i = 0; i < num_subjects; i++) {
                    subject_order[i] = i;
                }
                
                shuffleSubjects(subject_order, num_subjects);
                
                // Try to schedule all subjects
                bool all_subjects_scheduled = true;
                for (int i = 0; i < num_subjects; i++) {
                    if (!schedulePattern(sec, &subjects[subject_order[i]])) {
                        all_subjects_scheduled = false;
                        break;
                    }
                }
                
                if (all_subjects_scheduled) {
                    section_complete = true;
                    printf("Section %d scheduled successfully!\n", sec + 1);
                } else {
                    section_attempts++;
                }
            }
            
            if (!section_complete) {
                all_sections_complete = false;
                break;
            }
        }
        
        if (all_sections_complete) {
            complete = true;
            printf("All sections scheduled successfully!\n");
        } else {
            total_attempts++;
            printf("Attempt %d failed, retrying...\n", total_attempts);
        }
    }
    
    if (!complete) {
        printf("Failed to generate complete timetable after %d attempts\n", total_attempts);
    }
}

void printTimetable()
{
    const char *days[] = {"MON", "TUE", "WED", "THU", "FRI"};

    for (int sec = 0; sec < MAX_SECTIONS; sec++)
    {
        printf("\nTIMETABLE FOR SECTION %c\n", 'A' + sec);
        printf("==================================================\n");

        // Print header
        printf("     ");
        for (int slot = 0; slot < SLOTS_PER_DAY; slot++)
        {
            printf("%-15d", slot + 1);
        }
        printf("\n");

        // Print timetable for each day
        for (int day = 0; day < DAYS_PER_WEEK; day++)
        {
            printf("%-4s ", days[day]);

            for (int slot = 0; slot < SLOTS_PER_DAY; slot++)
            {
                if (strlen(timetable[sec][day][slot].name) > 0)
                {
                    char slot_info[20] = "";
                    strcpy(slot_info, timetable[sec][day][slot].name);
                    if (timetable[sec][day][slot].is_lab)
                    {
                        strcat(slot_info, "(L)");
                    }
                    else if (timetable[sec][day][slot].is_tutorial)
                    {
                        strcat(slot_info, "(T)");
                    }
                    if (strlen(timetable[sec][day][slot].faculty_initials) > 0)
                    {
                        strcat(slot_info, "-");
                        strcat(slot_info, timetable[sec][day][slot].faculty_initials);
                    }
                    printf("%-15s", slot_info);
                }
                else
                {
                    printf("%-15s", "---");
                }
            }
            printf("\n");
        }
        printf("\n");
    }
}

void printFacultyTimetables()
{
    const char *days[] = {"MON", "TUE", "WED", "THU", "FRI"};
    UniqueFaculty unique_faculties[MAX_SUBJECTS * MAX_FACULTY];
    int faculty_count = getUniqueFaculties(unique_faculties);

    printf("\nFACULTY TIMETABLES\n");
    printf("==================\n\n");

    // For each faculty
    for (int f = 0; f < faculty_count; f++)
    {
        printf("\nTIMETABLE FOR FACULTY: %s\n", unique_faculties[f].initials);
        printf("==================================================\n");

        // Print header
        printf("     ");
        for (int slot = 0; slot < SLOTS_PER_DAY; slot++)
        {
            printf("%-15d", slot + 1);
        }
        printf("\n");

        // Print timetable for each day
        for (int day = 0; day < DAYS_PER_WEEK; day++)
        {
            printf("%-4s ", days[day]);

            for (int slot = 0; slot < SLOTS_PER_DAY; slot++)
            {
                bool slot_found = false;
                // Check all sections for this faculty's classes
                for (int sec = 0; sec < MAX_SECTIONS; sec++)
                {
                    if (strcmp(timetable[sec][day][slot].faculty_initials,
                               unique_faculties[f].initials) == 0)
                    {
                        // Format: Subject(type)-Section
                        char slot_info[20];
                        sprintf(slot_info, "%s%s-%c",
                                timetable[sec][day][slot].name,
                                timetable[sec][day][slot].is_lab ? "(L)" : (timetable[sec][day][slot].is_tutorial ? "(T)" : ""),
                                'A' + sec);
                        printf("%-15s", slot_info);
                        slot_found = true;
                        break;
                    }
                }
                if (!slot_found)
                {
                    printf("%-15s", "---");
                }
            }
            printf("\n");
        }
        printf("\n");
    }
}
int main()
{
    generateTimetable();
    printTimetable();
    printFacultyTimetables();
    return 0;
}
