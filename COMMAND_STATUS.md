# IRC Server - Command Implementation Status

## Overview
This document tracks the implementation status of all IRC commands and identifies missing or incomplete features.

---

## ✅ COMPLETED COMMANDS

### 1. TOPIC Command
**Status:** ✅ Complete
- All error checks implemented
- View and set functionality working
- MODE_T restriction properly enforced
- Proper IRC numeric replies (RPL_TOPIC, RPL_NOTOPIC)

### 2. PING Command
**Status:** ✅ Complete
- Token handling implemented
- Proper PONG response
- Error handling for no origin

### 3. QUIT Command
**Status:** ✅ Complete
- Broadcasts quit message to all channels
- Closes connection properly
- Adds to departed clients list
- Removes client from server

---

## ⚠️ INCOMPLETE OR BUGGY COMMANDS

### 4. NICK Command
**Status:** ⚠️ Mostly Complete

**Issues:**
- ✅ Nickname validation works
- ✅ Duplicate check works
- ❌ **Missing:** Should trigger registration check after setting nickname (with USER)
- ❌ **Missing:** Should broadcast nick change to all channels the user is in
- ❌ **Missing:** Inconsistent numeric reply format (uses `:server` instead of `:localhost`)

**What's needed:**
```cpp
// After setting nickname, should broadcast to all user's channels:
const std::map<std::string, Channel *>& channels = client.getChannelList();
for (iterator...) {
    channel->sendMessageToClients(client.getFd(), nickChangeMsg);
}

// Should also trigger registration if NICK + USER + AUTH are complete
```

---

### 5. PASS Command
**Status:** ⚠️ Working but format issues

**Issues:**
- ✅ Password verification works
- ❌ **Missing:** Inconsistent reply format (doesn't follow IRC protocol format)
- ❌ Should use `:localhost` prefix and proper `\r\n` endings

**What's needed:**
```cpp
// Current: "461 ERR_NEEDMOREPARAMS PASS :Not enough parameters"
// Should be: ":localhost 461 <nick> PASS :Not enough parameters\r\n"
```

---

### 6. USER Command
**Status:** ⚠️ Working but incomplete

**Issues:**
- ✅ Basic registration works
- ❌ **Missing:** Should check if client is authenticated (PASS command first)
- ❌ **Missing:** Args[1] and args[2] are ignored (mode and unused parameter)
- ❌ **Missing:** No validation of username format

**What's needed:**
```cpp
// Should verify authentication first:
if (!client.isAuthenticated()) {
    client.sendMessage(":localhost 464 * :Password required\r\n");
    return;
}
```

---

### 7. JOIN Command
**Status:** ⚠️ Partially working - CRITICAL ISSUES

**Issues:**
- ✅ Basic join works
- ✅ Password check works (+k mode)
- ❌ **CRITICAL:** No invite-only mode check (+i)
- ❌ **CRITICAL:** No user limit check (+l)
- ❌ **CRITICAL:** First user joining should automatically become operator
- ❌ **MISSING:** No invite list verification
- ❌ **MISSING:** Doesn't broadcast JOIN to all channel members properly

**What's needed:**
```cpp
// Check invite-only mode
if (channel->getChannelMode(MODE_I)) {
    if (!channel->isInvited(&client)) {
        client.sendMessage(":localhost 473 " + client.getNickname() + " " + channelName + " :Cannot join channel (+i)\r\n");
        return;
    }
}

// Check user limit
if (channel->getChannelMode(MODE_L)) {
    if (channel->getClients().size() >= channel->getClientLimit()) {
        client.sendMessage(":localhost 471 " + client.getNickname() + " " + channelName + " :Cannot join channel (+l)\r\n");
        return;
    }
}

// Make first user operator
if (channel->getClients().empty()) {
    channel->setOperator(&client);
}
```

---

### 8. PART Command
**Status:** ⚠️ Partially working

**Issues:**
- ✅ Basic leave functionality works
- ❌ **MISSING:** No registration check
- ❌ **MISSING:** No channel existence check
- ❌ **MISSING:** No "user on channel" check
- ❌ **MISSING:** No error handling for NULL channel pointers
- ❌ **MISSING:** Multiple channel support (comma-separated) not tested

**What's needed:**
```cpp
// Add error checks:
if (!client.isRegistered()) {
    client.sendMessage(":localhost 451 * :You have not registered\r\n");
    return;
}

if (args.empty()) {
    client.sendMessage(":localhost 461 " + client.getNickname() + " PART :Not enough parameters\r\n");
    return;
}

// Check if channel exists before using it
if (!channel) {
    client.sendMessage(":localhost 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
    continue;
}

// Check if user is on channel
if (!channel->isClient(&client)) {
    client.sendMessage(":localhost 442 " + client.getNickname() + " " + channelName + " :You're not on that channel\r\n");
    continue;
}
```

---

### 9. KICK Command
**Status:** ✅ Mostly Complete

**Issues:**
- ✅ All error checks implemented
- ✅ Operator verification works
- ⚠️ **Minor:** Could benefit from checking if target is kickable (can't kick yourself)

---

### 10. INVITE Command
**Status:** ⚠️ CRITICAL BUGS

**Issues:**
- ✅ Basic structure exists
- ❌ **CRITICAL BUG:** Searches for target user WITHIN channel members only
- ❌ **MISSING:** Should search ALL server clients, not just channel members
- ❌ **MISSING:** No operator check when channel is invite-only (+i)
- ❌ **MISSING:** No invite list storage (invites are not persisted)
- ❌ **WRONG LOGIC:** Checks if target is IN channel (should be opposite)

**Current bug (line 44-51):**
```cpp
// BUG: Only searches within channel members
for (std::map<int, Client *>::const_iterator clientIt = clients.begin(); clientIt != clients.end(); ++clientIt)
{
    if (clientIt->second->getNickname() == targetNick) {
        targetClient = clientIt->second;
        break;
    }
}
```

**Should be:**
```cpp
// Search ALL server clients
targetClient = server.getClient(targetNick);

if (!targetClient) {
    client.sendMessage(":localhost 401 " + client.getNickname() + " " + targetNick + " :No such nick/channel\r\n");
    return;
}

// Check if already on channel
if (channel->isClient(targetClient)) {
    client.sendMessage(":localhost 443 " + client.getNickname() + " " + targetNick + " " + channelName + " :is already on channel\r\n");
    return;
}

// Check if operator (when +i mode)
if (channel->getChannelMode(MODE_I) && !channel->isOperator(&client)) {
    client.sendMessage(":localhost 482 " + client.getNickname() + " " + channelName + " :You're not channel operator\r\n");
    return;
}

// Add to invite list
channel->addInvite(targetClient);
```

---

### 11. PRIVMSG Command
**Status:** ⚠️ Working but incomplete

**Issues:**
- ✅ Basic message sending works (to users and channels)
- ❌ **MISSING:** No registration check
- ❌ **MISSING:** No check if user is on channel when sending to channel
- ❌ **MISSING:** No moderation check (can't send if +m and not operator/voiced)
- ❌ **Comment in code:** "if channel add @ if target is op" - not implemented

**What's needed:**
```cpp
if (!client.isRegistered()) {
    client.sendMessage(":localhost 451 * :You have not registered\r\n");
    return;
}

// For channel messages - check if user is on channel
if (getType(target_name)) {
    Channel *target = server.getChannel(target_name);
    if (!target) {
        client.sendMessage(":localhost 403 " + client.getNickname() + " " + target_name + " :No such channel\r\n");
        return;
    }
    
    // Check if user is on channel
    if (!target->isClient(&client)) {
        client.sendMessage(":localhost 404 " + client.getNickname() + " " + target_name + " :Cannot send to channel\r\n");
        return;
    }
    
    target->sendMessageToClients(client.getFd(), fullMsg);
}
```

---

### 12. MODE Command
**Status:** ❌ INCOMPLETE - CRITICAL ISSUES

**Issues:**
- ✅ Basic structure exists
- ✅ Mode parsing logic present
- ❌ **CRITICAL:** All error checks just use `std::cout` - no messages sent to client!
- ❌ **CRITICAL:** Wrong registration check (line 16: `if (client.isRegistered())` should be `!`)
- ❌ **CRITICAL:** Wrong operator check (line 32: `if (channel->isClient(&client))` should be `!channel->isOperator(&client)`)
- ❌ **MISSING:** No mode change broadcast to channel members
- ❌ **MISSING:** No reply showing current modes when no mode parameter given
- ❌ **MISSING:** Missing error messages for:
  - No password provided for +k
  - Invalid user for +o
  - No limit provided for +l

**Complete rewrite needed:**
```cpp
void ModeCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
    // 1. Check registration
    if (!client.isRegistered()) {
        client.sendMessage(":localhost 451 * :You have not registered\r\n");
        return;
    }

    // 2. Check parameters
    if (args.empty()) {
        client.sendMessage(":localhost 461 " + client.getNickname() + " MODE :Not enough parameters\r\n");
        return;
    }

    std::string channelName = args[0];
    Channel *channel = server.getChannel(channelName);

    // 3. Check channel exists
    if (!channel) {
        client.sendMessage(":localhost 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return;
    }

    // 4. If no mode string - show current modes
    if (args.size() == 1) {
        std::string modeStr = "+";
        if (channel->getChannelMode(MODE_I)) modeStr += "i";
        if (channel->getChannelMode(MODE_T)) modeStr += "t";
        if (channel->getChannelMode(MODE_K)) modeStr += "k";
        if (channel->getChannelMode(MODE_L)) modeStr += "l";
        
        client.sendMessage(":localhost 324 " + client.getNickname() + " " + channelName + " " + modeStr + "\r\n");
        return;
    }

    // 5. Check operator privileges for mode changes
    if (!channel->isOperator(&client)) {
        client.sendMessage(":localhost 482 " + client.getNickname() + " " + channelName + " :You're not channel operator\r\n");
        return;
    }

    // 6. Parse and apply modes
    std::string modes = args[1];
    std::vector<std::string>::const_iterator paramIt = args.begin() + 2;
    bool adding = true;
    std::string modeChanges;
    std::string modeParams;

    for (std::string::iterator it = modes.begin(); it != modes.end(); ++it) {
        char mode = *it;
        
        if (mode == '+') {
            adding = true;
            continue;
        } else if (mode == '-') {
            adding = false;
            continue;
        }

        // Handle each mode
        switch (mode) {
            case 'i':
                channel->setChannelMode(MODE_I, adding);
                modeChanges += adding ? "+i" : "-i";
                break;
                
            case 't':
                channel->setChannelMode(MODE_T, adding);
                modeChanges += adding ? "+t" : "-t";
                break;
                
            case 'k':
                if (adding) {
                    if (paramIt == args.end()) {
                        client.sendMessage(":localhost 461 " + client.getNickname() + " MODE :Not enough parameters\r\n");
                        return;
                    }
                    channel->setPassword(*paramIt);
                    modeChanges += "+k";
                    modeParams += " " + *paramIt;
                    ++paramIt;
                } else {
                    channel->setPassword("");
                    channel->setChannelMode(MODE_K, false);
                    modeChanges += "-k";
                }
                break;
                
            case 'o':
                if (paramIt == args.end()) {
                    client.sendMessage(":localhost 461 " + client.getNickname() + " MODE :Not enough parameters\r\n");
                    return;
                }
                Client *target = server.getClient(*paramIt);
                if (!target) {
                    client.sendMessage(":localhost 401 " + client.getNickname() + " " + *paramIt + " :No such nick/channel\r\n");
                    ++paramIt;
                    continue;
                }
                if (!channel->isClient(target)) {
                    client.sendMessage(":localhost 441 " + client.getNickname() + " " + *paramIt + " " + channelName + " :They aren't on that channel\r\n");
                    ++paramIt;
                    continue;
                }
                if (adding) {
                    channel->setOperator(target);
                    modeChanges += "+o";
                } else {
                    channel->removeOperator(target);
                    modeChanges += "-o";
                }
                modeParams += " " + *paramIt;
                ++paramIt;
                break;
                
            case 'l':
                if (adding) {
                    if (paramIt == args.end()) {
                        client.sendMessage(":localhost 461 " + client.getNickname() + " MODE :Not enough parameters\r\n");
                        return;
                    }
                    if (!isStringNumeric(*paramIt)) {
                        ++paramIt;
                        continue;
                    }
                    size_t limit = atol((*paramIt).c_str());
                    channel->setClientLimit(limit);
                    channel->setChannelMode(MODE_L, true);
                    modeChanges += "+l";
                    modeParams += " " + *paramIt;
                    ++paramIt;
                } else {
                    channel->setClientLimit(0);
                    channel->setChannelMode(MODE_L, false);
                    modeChanges += "-l";
                }
                break;
                
            default:
                client.sendMessage(":localhost 472 " + client.getNickname() + " " + std::string(1, mode) + " :is unknown mode char to me\r\n");
                break;
        }
    }

    // 7. Broadcast mode change
    if (!modeChanges.empty()) {
        std::string modeMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() + 
                             " MODE " + channelName + " " + modeChanges + modeParams + "\r\n";
        channel->sendMessageToClients(-1, modeMsg);
    }
}
```

---

### 13. PONG Command
**Status:** ⚠️ Questionable implementation

**Issues:**
- ⚠️ Implementation seems odd - ignores args
- ⚠️ PONG is typically auto-generated, not a user command
- ⚠️ Should this even be a user-accessible command?

---

## 🚫 MISSING COMMANDS (Not Implemented)

### Required Commands Not Found:
1. **CAP** - Capability negotiation (partially handled in ServerNetwork.cpp)
2. **WHO** - List users (might be required depending on subject)
3. **WHOIS** - User information (commented in code, not implemented)
4. **LIST** - List channels
5. **NAMES** - List channel members (partially in JOIN, but no standalone command)

---

## 🏗️ MISSING INFRASTRUCTURE

### Channel Features:
1. ❌ **Invite List Storage**
   - `Channel` class has no `_inviteList` member
   - Cannot track who is invited to +i channels
   - Need to add:
   ```cpp
   private:
       std::vector<Client*> _inviteList;
   
   public:
       void addInvite(Client* client);
       void removeInvite(Client* client);
       bool isInvited(Client* client) const;
   ```

2. ❌ **First User Auto-Operator**
   - When creating a new channel, first user should become operator
   - Currently not implemented in JOIN command

3. ❌ **Channel Cleanup**
   - Empty channels should be deleted
   - No cleanup mechanism exists

4. ✅ **User Limit** - Structure exists (`_clientLimit` in Channel)

### Server Features:
1. ❌ **removeClient() implementation**
   - Declared in Server.hpp but not implemented
   - Called in QuitCommand - will cause linker error

---

## 📊 PRIORITY FIX LIST

### Critical (Must Fix):
1. **MODE Command** - Complete rewrite needed, currently unusable
2. **INVITE Command** - Fix client search logic
3. **JOIN Command** - Add +i and +l checks, first user operator
4. **Server::removeClient()** - Implement missing function

### High Priority:
5. **PART Command** - Add all error checks
6. **PRIVMSG Command** - Add registration and channel membership checks
7. **Channel::addInvite/isInvited** - Implement invite list infrastructure

### Medium Priority:
8. **NICK Command** - Add broadcast to channels
9. **USER Command** - Add authentication check
10. **PASS Command** - Fix reply format consistency

### Low Priority:
11. **PONG Command** - Review if needed as user command
12. Format consistency across all commands (`:localhost` prefix, `\r\n` endings)

---

## 📝 NOTES

- Commands marked with `(void)` parameters are suppressing unused variable warnings
- Many commands have helpful comments but incomplete implementations
- Error messages are inconsistent (some use numeric codes, some don't)
- Some commands use `std::cout` for debugging instead of sending client messages

---

## ✅ TESTING CHECKLIST

Once fixes are complete, test:
- [ ] Can create channel and become operator
- [ ] Can set all modes (+i, +t, +k, +o, +l)
- [ ] +i mode prevents non-invited users from joining
- [ ] +l mode enforces user limit
- [ ] +k mode requires correct password
- [ ] +t mode restricts topic changes to operators
- [ ] INVITE adds user to invite list and allows +i join
- [ ] Multiple mode changes in one command work
- [ ] MODE without parameters shows current modes
- [ ] PART with reason broadcasts correctly
- [ ] KICK removes user and broadcasts
- [ ] PRIVMSG checks channel membership
- [ ] TOPIC respects +t mode
- [ ] Empty channels are deleted
- [ ] All error codes are correct and consistent
