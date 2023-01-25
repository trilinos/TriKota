#/usr/bin/env python3
"""
Trilinos Status Table Automation Script
      
      table_Automation.py <pr_status, pr_merged, failed_pr, 
                                waiting_pr, open_pr, mm_status, successful_mm[, jira_ticket]>
    
      This script prints out todays Trilinos status in MarkDown syntax so that it can be added to the Trilinos Wiki page.
    
** Dependencies needed to run script:
    
    pip3 install python-dateutil
    pip3 install pytablewriter
    
    
SYNOPSIS                                                                                                                                 
<python> table_Automation.py <0 4 6 4 72 0 4 [435]>

#STATUS: [0] == success, [1] == warning [3] == failure -> Pull Request(s) and Master Merge Status codes.

updated table string example: python table_Automation2.py 0 1 2 3 4 0 5 6 7 8 9 10 111
                              python table_Automation2.py 1 3 1 22 21 0 5 5 21 58 1 504
"""
from time import sleep as pause
from datetime import timedelta

from datetime import date
import subprocess
import sys
import os

STOP = 0b11
ONE_OFF = -0b1

try:
  import pytablewriter as ptw
  from dateutil.relativedelta import relativedelta, FR
except ImportError:
  print("\n\nHere are a list of needed dependencies ... \n\n\n")
  pause(STOP)

  print("\t*** Package(s) that need to be installed ***\n \n\tpip3 install python-dateutil " 
        + "\n\tpip3 install pytablewriter \n\n\n\t*** Install and try running again ***\n\n\n")
  pause(STOP)  

def main():
    #STATUS: [0] == success, [1] == warning [3] == failure
    mm_status = [":white_check_mark:", ":warning:", ":x:"]
    pr_status = [":white_check_mark:", ":warning:", ":x:"]
    status_example = ":white_check_mark:"
    
    stat_container = sys.argv[1:]
   
    today = date.today()
    yesterday = today - timedelta(days = 1)
    try:
        last_Friday = today + relativedelta(weekday=FR(ONE_OFF))
    
    except:
        print("Veiw needed dependencies above\n\n")

    try:
        number_of_pr_merged = stat_container[1]
        number_of_failed_pr = stat_container[2]
        
        # new table items
        # WIP_Pr - complete
        number_wip_prs = stat_container[3]
        
        # review_required - complete
        number_reviewed_required = stat_container[4]

        # change_requested - complete
        number_change_requested = stat_container[6]

        # review_approved - complete
        number_review_approved = stat_container[7]

        # old update index
        number_of_waiting_pr = stat_container[8]
        number_open_pr = stat_container[9] # update to failed pr's @ 12 options
        number_of_successful_mm = stat_container[10]
        jira_ticket_number = stat_container[11]
    except:
        print("Requires more arguments ... Example: table_Automation.py 0 4 6 4 72 0 4 435")
    try:
        NUMBER_OF_PRs_MERGED = "["+number_of_pr_merged+"]"+"(https://github.com/trilinos/Trilinos/pulls?q=is%3Apr+merged%3A"+str(yesterday)+"T12%3A00%3A00-07%3A00.."+str(today)+"T12%3A00%3A00-07%3A00+base%3Adevelop)"
        # -> NUMBER_OF_FAILED_PRs = "["+number_of_failed_pr+"]"+"(https://github.com/trilinos/Trilinos/pulls?q=is%3Apr+updated%3A"+str(yesterday)+"T12%3A00%3A00-07%3A00.."+str(today)+"T12%3A00%3A00-07%3A00+base%3Adevelop+status%3Afailure+)"
        # NUMBER_OF_PRs_WAITING = "["+number_of_waiting_pr+"]"+"(https://github.com/trilinos/Trilinos/pulls?q=is%3Apr+is%3Aopen+base%3Adevelop+status%3Apending+-label%3A%22AT%3A+WIP%22+)"
        # -> NUMBER_OF_PRs_WAITING = "["+number_of_waiting_pr+"]"+"(https://github.com/trilinos/Trilinos/pulls?q=is%3Apr+is%3Aopen+base%3Adevelop+review%3Aapproved+status%3Afailure+-label%3A%22AT%3A+WIP%22)"
        NUMBER_SUCCESSFUL_MM = "["+number_of_successful_mm+"]"+"(https://github.com/trilinos/Trilinos/pulls?q=is%3Apr+merged%3A"+str(last_Friday)+"T12%3A00%3A00-07%3A00.."+str(today)+"T12%3A00%3A00-07%3A00+base%3Amaster+)"
        JIRA_TICKETS = "[TrilFrame-"+jira_ticket_number+"]"+"(https://sems-atlassian-son.sandia.gov/jira/browse/TRILFRAME-"+str(jira_ticket_number)+")"
        # change "total open PR's at 12" to clickable link
        Open_PRs = "["+number_open_pr+"]"+"(https://github.com/trilinos/Trilinos/pulls)"
        #   new table items 
        WIP_PRs = "["+number_wip_prs+"]"+"(https://github.com/trilinos/Trilinos/pulls?q=+is%3Apr+is%3Aopen+base%3Adevelop+label%3A%22AT%3A+WIP%22)"
        REVIEW_REQUIRED = "["+number_reviewed_required+"]"+"(https://github.com/trilinos/Trilinos/pulls?q=is%3Apr+is%3Aopen+base%3Adevelop+review%3Arequired+-label%3A%22AT%3A+WIP%22)"
        CHANGE_REQUESTED = "["+number_change_requested+"]"+"(https://github.com/trilinos/Trilinos/pulls?q=+is%3Apr+is%3Aopen+base%3Adevelop+review%3Achanges-requested+-label%3A%22AT%3A+WIP%22)"
        REVIEW_APROVED = "["+number_review_approved+"]"+"(https://github.com/trilinos/Trilinos/pulls?q=+is%3Apr+is%3Aopen+base%3Adevelop+review%3Aapproved+-status%3Afailure+-label%3A%22AT%3A+WIP%22)"
    except:
        print("Missing required argument(s)... Example: table_Automation.py 0 4 6 4 72 0 4 435")
    try:
        writer = ptw.MarkdownTableWriter(
              table_name="Trilinos Status Table",
              headers=["Date", "PR Status", "PRs Merged (Past 24 Hrs from 12pm)", "WIP PRs (@ 12pm)", "Review-Required PRs (@ 12pm) ", "Change-Requested PRs (@ 12pm) ", "Review-Approved PRs (@ 12pm)", "Total Open PRs (@ 12pm)", "MM Status", "Master Merges (Past 24 hrs from 12pm)", "Jira Ticket #"],
              value_matrix=[
                    [str(today), pr_status[int(stat_container[0])], NUMBER_OF_PRs_MERGED, WIP_PRs, REVIEW_REQUIRED, CHANGE_REQUESTED, REVIEW_APROVED, Open_PRs , mm_status[int(stat_container[5])], NUMBER_SUCCESSFUL_MM, JIRA_TICKETS]
              ],
          )
    except:
        print("Not enough arguments ... Example: table_Automation.py 0 4 6 4 72 0 4 435")
    #change the output stream to terminal <- OPTIONAL IMPORT ABOVE
    # writer.stream = io.StringIO()
    # writer.write_table()
    # print(writer.stream.getvalue())

    # change the output stream to a file
    try:
        print(writer)
        # with open("trilinos_Table_Updates.md", "w") as f:
            # writer.stream = f
            # writer.write_table()
            # or you can use dump method to file if you just output a table to a file
            # writer.dump("sample.md")
    except:
        print("Can not write to file, missing arguements ... Example: table_Automation.py 0 4 6 4 72 0 4 435")

'''
   -> python table_Automation2.py 0 1 2 3 4 0 5 6 7 8 9 10 -> working
   python table_Automation2.py 1 3 1 22 21 0 5 5 21 58 1 504 <twelve input values from user>
'''    
if __name__ == "__main__":
    main()



'''
    1) Let’s just drop the “Failed PRs (Past 24 Hrs from 12pm)” column.  
    It would be nice to show how many PRs failed in the last 24 hours but this search reports 
    something slightly different (i.e., PRs that have been updated in the last 
    24 hours that have the failed status at 12pm) which requires explanation and just causes 
    confusion. If we can figure out how to report “PRs failed in the last 24 hours”, 
    we should add this back.  
    - Also, the “Failed PRs (@ 12pm)” column reports the number of 
    failures @ 12pm which can give us a felling if things are backing up.


    2 COMPLETE) Is there a reason why the “Total Open PRs” is not a link in the table?  
    If possible, can you make it a link?
    
    
    3 COMPLETE) Let’s change the column header “Successful Master Merges” to “Master Merges 
    (Past 24 hrs from 12pm)” to match the others.

    

'''