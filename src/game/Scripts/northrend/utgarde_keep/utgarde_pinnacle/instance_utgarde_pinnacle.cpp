/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: instance_pinnacle
SD%Complete: 25%
SDComment:
SDCategory: Utgarde Pinnacle
EndScriptData */

#include "pchdef.h"
#include "utgarde_pinnacle.h"

instance_pinnacle::instance_pinnacle(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_pinnacle::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_pinnacle::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_DOOR_SKADI:
            if (m_auiEncounter[TYPE_SKADI] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_mGoEntryGuidStore[GO_DOOR_SKADI] = pGo->GetObjectGuid();
            break;
    }
}

void instance_pinnacle::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_SVALA:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_GORTOK:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_SKADI:
            if (uiData == DONE)
                DoUseDoorOrButton(GO_DOOR_SKADI);

            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_YMIRON:
            m_auiEncounter[uiType] = uiData;
            break;
        default:
            error_log("SSC: Instance Pinnacle: SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
            return;
    }

    // Saving also SPECIAL for this instance
    if (uiData == DONE || uiData == SPECIAL)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_pinnacle::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_pinnacle::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_pinnacle(Map* pMap)
{
    return new instance_pinnacle(pMap);
}

void AddSC_instance_pinnacle()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_pinnacle";
    pNewScript->GetInstanceData = &GetInstanceData_instance_pinnacle;
    pNewScript->RegisterSelf();
}
