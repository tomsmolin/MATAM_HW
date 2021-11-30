#### IMPORTS ####
import event_manager as EM


#### PART 1 ####
# Filters a file of students' subscription to specific event:
#   orig_file_path: The path to the unfiltered subscription file
#   filtered_file_path: The path to the new filtered file

def legalID(member_id: int) -> bool:
    if member_id < 10000000 or member_id >= 100000000:
        return False
    return True

def legalName(member_name: str, name_parts_list: list) -> bool:
    i = 0
    space = ' '
    while len(member_name) > 0:
        # The current char is not a letter
        if member_name[i] != space and not ('a' <= member_name[i] <= 'z' or 'A' <= member_name[i] <= 'Z'):
            return False

        if member_name[i] == space:
            name = member_name[0: i]
            name_parts_list.append(name)
            member_name = member_name[(i + 1):].strip()
            i = 0
            continue
            # Use strip() on the remain part of the name to remove spaces from the edges.
        if i < len(member_name):
            i += 1
        if i == len(member_name):
            name_parts_list.append(member_name)
            member_name = ''

    return True


def legalAge(member_age: int) -> bool:
    if member_age < 16 or member_age > 120:
        return False
    return True


def yearAndBirthMatch(year_of_birth: int, member_age: int) -> bool:
    estimated_age = 2020 - year_of_birth
    if estimated_age != member_age:
        return False
    return True


def legalSemester(semester: int) -> bool:
    if semester < 1:
        return False

    return True


def lineLegal(member_details: list) -> bool:
    # Check - if the Id is legal
    member_id = int(member_details[0])
    if not legalID(member_id):
        return False
    member_details[0] = member_details[0].strip()

    # Check - if the Name is legal
    member_name = member_details[1].strip()  # We should use strip to clean the string from spaces at the begining
    name_parts_list = []

    if not legalName(member_name, name_parts_list):
        return False
    separator = ' '
    member_details[1] = separator.join(name_parts_list)

    # Check - if the age is Legal
    member_age = int(member_details[2])
    if not legalAge(member_age):
        return False
    member_details[2] = member_details[2].strip()

    # Check - if the age matches the year of birth
    year_of_birth = int(member_details[3])
    if not yearAndBirthMatch(year_of_birth, member_age):
        return False
    member_details[3] = member_details[3].strip()

    # Check - if the semester is legal
    semester = int(member_details[4])
    if not legalSemester(semester):
        return False
    member_details[4] = member_details[4].strip()

    return True


def getYoungestMemberLine(lines: list) -> str:
    youngest_member_line = lines[0]
    member_details = youngest_member_line.split(',')
    youngest_member_age = int(member_details[2])
    iterations = len(lines) - 1 # Avoid from getting OOB
    for i in range(iterations):
        age_compare = lines[i + 1].split(',')
        if youngest_member_age > int(age_compare[2]):
            youngest_member_age = int(age_compare[2])
            youngest_member_line = lines[i + 1]

    return youngest_member_line


def getKYoungestMembers(out_file_path: str, lines: list, k: int) -> int:
    youngest_list = open(out_file_path, 'w')
    names_counter = 0
    for i in range(k):
        youngest = getYoungestMemberLine(lines)
        youngest_details = youngest.split(',')
        new_line = youngest_details[1].strip() + '\n'
        youngest_list.write(new_line)
        names_counter += 1
        lines.remove(youngest)
        if len(lines) == 0:
            youngest_list.close()
            return names_counter

    youngest_list.close()
    return names_counter


def fileCorrectToList(orig_file_path: str, correct_lines: list, legal_members_id_arr: list):
    original_file = open(orig_file_path, 'r')
    count = 0

    for line in original_file:
        count += 1
    original_file.close()

    # check lines:
    original_file = open(orig_file_path, 'r')
    for line in range(count):

        member_details = (original_file.readline()).split(',')
        member_id = int(member_details[0])

        if lineLegal(member_details):

            if member_id not in legal_members_id_arr:
                legal_members_id_arr.append(member_id)
            legal_members_id_arr.sort()
            separator = ', '
            new_line = separator.join(member_details)
            correct_lines.append(new_line + '\n')

    original_file.close()


def printToFileById(lines: list, filtered_file_path: str, legal_members_id: list):
    filtered = open(filtered_file_path, 'w')
    lines.reverse()

    # iterating over the people to print
    for current_member in range(len(legal_members_id)):
        # Looking for the first appearance of 'current member' in 'lines'
        for i in range(len(lines)):
            member_details = (lines[i]).split(',')
            current_id = int(member_details[0])
            if legal_members_id[current_member] == current_id:
                member_details = lines[i]
                filtered.write(member_details)
                break

    filtered.close()


def filterBySemster(lines: list, semester: int, legal_members_id: list) -> list:
    new_lines = []
    # The last insertion of information for each member is the one that counts
    lines.reverse()
    # Looking for members with relevant id
    for member in range(len(legal_members_id)):

        for i in range(len(lines)):
            member_details = (lines[i]).split(',')
            current_member_semester = int(member_details[4])
            current_member_id = int(member_details[0])
            if current_member_id == legal_members_id[member]:

                if semester == current_member_semester:
                    new_lines.append(lines[i])
                # Only the last line for each member counts
                break

    return new_lines


def membersAgeSum(members_list: list) -> int:
    ages_sum = 0

    for i in range(len(members_list)):
        member_details = members_list[i].split(',')
        member_age = int(member_details[2])
        ages_sum += member_age

    return ages_sum


def fileCorrect(orig_file_path: str, filtered_file_path: str):
    correct_lines = []
    legal_members_id_arr = []
    fileCorrectToList(orig_file_path, correct_lines, legal_members_id_arr)
    printToFileById(correct_lines, filtered_file_path, legal_members_id_arr)


# Writes the names of the K youngest students which subscribed
# to the event correctly.
#   in_file_path: The path to the unfiltered subscription file
#   out_file_path: file path of the output file
def printYoungestStudents(in_file_path: str, out_file_path: str, k: int) -> int:
    if k < 1:
        return -1
    fileCorrect(in_file_path, out_file_path)
    filtered_file = open(out_file_path, 'r')
    lines_array = filtered_file.readlines()
    if len(lines_array) == 0:
        return 0
    filtered_file.close()
    names_counter = getKYoungestMembers(out_file_path, lines_array, k)

    return names_counter


# # Calculates the avg age for a given semester
# #   in_file_path: The path to the unfiltered subscription file
# #   retuns the avg, else error codes defined.
def correctAgeAvg(in_file_path: str, semester: int) -> float:
    if not (legalSemester(semester)):
        return -1

    correct_lines = []
    legal_members_id_arr = []
    fileCorrectToList(in_file_path, correct_lines, legal_members_id_arr)
    if len(correct_lines) == 0:
        return 0
    filtered_list_by_semester = filterBySemster(correct_lines, semester, legal_members_id_arr)
    members_in_semester = len(filtered_list_by_semester)
    if members_in_semester == 0:
        return 0

    ages_sum = membersAgeSum(filtered_list_by_semester)
    average_age = ages_sum / members_in_semester

    return average_age


# #### PART 2 ####
# # Use SWIG :)
# # print the events in the list "events" using the functions from hw1
# #   events: list of dictionaries
# #   file_path: file path of the output file
def addEvents(em, events: list):
    for i in range(len(events)):
        event_name = events[i]['name']
        event_date = events[i]['date']
        event_id = events[i]['id']
        EM.emAddEventByDate(em, event_name, event_date, event_id)


def getFirstDate(events: list):
    first_date = events[0]['date']

    for i in range(len(events)):
        current_date = events[i]['date']
        if EM.dateCompare(first_date, current_date) > 0:
            first_date = current_date

    return first_date

#em, event_names: list, event_id_list: list, day: int, month: int, year: int
def printEventsList(events :list,file_path :str):
    first_date = getFirstDate(events)
    em = EM.createEventManager(first_date)
    addEvents(em, events)
    EM.emPrintAllEvents(em, file_path)

    return em


def testPrintEventsList(file_path :str):
    events_lists=[{"name":"New Year's Eve","id":1,"date": EM.dateCreate(30, 12, 2020)},\
                    {"name" : "annual Rock & Metal party","id":2,"date":  EM.dateCreate(21, 4, 2021)}, \
                                 {"name" : "Improv","id":3,"date": EM.dateCreate(13, 3, 2021)}, \
                                     {"name" : "Student Festival","id":4,"date": EM.dateCreate(13, 5, 2021)},    ]
    em = printEventsList(events_lists,file_path)
    for event in events_lists:
        EM.dateDestroy(event["date"])
    EM.destroyEventManager(em)

#### Main ####
# feel free to add more tests and change that section.
# sys.argv - list of the arguments passed to the python script
if __name__ == "__main__":
    import sys
    if len(sys.argv)>1:
        testPrintEventsList(sys.argv[1])