Based on my analysis of your IRC server implementation, here is a comprehensive list of what's missing or incomplete to comply with a typical IRC server subject (like the 42 ft_irc project):

Missing Commands
1. MODE Command ❌
Critical: No MODE command implementation at all
Should handle both channel modes and user modes
Required channel modes you have structure for but no command:
+i (invite-only)
+t (topic restriction to operators)
+k (channel password/key)
+o (give/take operator privileges)
+l (set/remove user limit)
2. TOPIC Command ❌
Critical: No TOPIC command implementation
Channel has topic getter/setter but no command to use them
Should allow:
Viewing current topic
Setting topic (with +t mode restriction check)
Incomplete Commands
3. PART Command ⚠️
Incomplete: Basic structure exists but not fully implemented
Missing:
Actually removing client from channel
Proper message formatting
Broadcasting to all channel members
Cleanup when channel becomes empty
4. INVITE Command ⚠️
Partially working but missing:
No invite list storage (for +i mode)
Client search is checking within channel members instead of all server clients
Should check if channel is invite-only (+i mode)
Should verify if user is operator (when +i mode is active)
5. JOIN Command ⚠️
Missing:
Invite-only mode (+i) check
User limit mode (+l) check
First user should automatically become operator
No invite list verification
Missing Channel Features
6. Channel User Limit ❌
MODE +l structure exists but no actual limit storage
No limit enforcement in JOIN command
Missing _userLimit member in Channel class
7. Invite List Management ❌
No invite list storage in Channel class
Cannot track who is invited to invite-only channels
MODE +i won't work properly without this
8. Channel Cleanup ⚠️
No automatic channel deletion when last user leaves
Channels persist even when empty
Missing/Incomplete Protocol Features
9. Numeric Replies ⚠️
Some commands use proper numerics, others don't
Inconsistent formatting across commands
Missing many standard IRC numeric replies
10. Channel Mode String ❌
No way to display current channel modes
MODE command needed to query current modes
11. WHO/WHOIS Commands ❌
No WHO command (may be required depending on subject)
No WHOIS command (may be required)
Note: You have "departed clients" tracking suggesting WHOIS was planned
12. Names List ⚠️
Names list exists in JOIN but:
Doesn't distinguish operators (@) correctly in display
No separate NAMES command
Code Quality Issues
13. Error Handling
Some commands lack proper error messages
Inconsistent error reply formats
14. PART Command Comments
Has TODOs in comments indicating incomplete implementation
15. Operator Assignment
JOIN doesn't automatically make the first user an operator
This is standard IRC behavior
Summary of Critical Missing Features
Must implement:

✅ MODE command - Most critical
✅ TOPIC command - Critical
✅ Complete PART command - Important
✅ User limit tracking and enforcement - For MODE +l
✅ Invite list management - For MODE +i
✅ First user auto-operator - Standard behavior
✅ Channel cleanup - Memory management
Should implement (depending on subject):

WHO/WHOIS commands
NAMES command (separate from JOIN)
Better numeric reply consistency