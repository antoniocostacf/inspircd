/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  InspIRCd: (C) 2002-2008 InspIRCd Development Team
 * See: http://www.inspircd.org/wiki/index.php/Credits
 *
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

#include "inspircd.h"
#include "xline.h"

#include "m_spanningtree/treesocket.h"
#include "m_spanningtree/treeserver.h"
#include "m_spanningtree/utils.h"

/* $ModDep: m_spanningtree/utils.h m_spanningtree/treeserver.h m_spanningtree/treesocket.h */


bool TreeSocket::DelLine(const std::string &prefix, std::deque<std::string> &params)
{
	if (params.size() < 2)
		return true;

	Instance->Logs->Log("loltree", DEBUG, "DELLINE: %s %s", params[0].c_str(), params[1].c_str());
	std::string setter = "<unknown>";

	User* user = Instance->FindNick(prefix);
	if (user)
		setter = user->nick;
	else
        {
		TreeServer* t = Utils->FindServer(prefix);
		if (t)
			setter = t->GetName().c_str();
	}


	/* NOTE: No check needed on 'user', this function safely handles NULL */
	if (Instance->XLines->DelLine(params[0].c_str(), params[1], user))
	{
		this->Instance->SNO->WriteToSnoMask('x',"%s removed %s%s on %s.", setter.c_str(),
				params[0].c_str(), params[0].length() == 1 ? "LINE" : "", params[1].c_str());
		Utils->DoOneToAllButSender(prefix,"DELLINE", params, prefix);
	}
	return true;
}

