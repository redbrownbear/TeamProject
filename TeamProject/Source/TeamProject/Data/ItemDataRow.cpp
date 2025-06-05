// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemDataRow.h"

eEquipParts FItemData::GetParts() const
{
	eEquipParts Parts = eEquipParts::NONE;

	switch (eItemCategory)
	{
	case EItemCategory::IT_Weapon:
		Parts = eEquipParts::RIGHT;
		break;
	case EItemCategory::IT_Shield:
		Parts = eEquipParts::LEFT;
		break;
	case EItemCategory::IT_Arrow:
		Parts = bIsArrow ? eEquipParts::ARROWLEFT : eEquipParts::BOWRIGHT;
		break;
	case EItemCategory::IT_Armor:
		switch (eArmorKind)
		{
		case EArmorKind::HEAD:
			Parts = eEquipParts::HEAD;
			break;
		case EArmorKind::ARMOR:
			Parts = eEquipParts::ARMOR;
			break;
		case EArmorKind::LEG:
			Parts = eEquipParts::UNDER;
			break;
		}
		break;
	}

	return Parts;
}
