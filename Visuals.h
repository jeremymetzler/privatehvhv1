#pragma once

class Visuals {
public:
	bool BoundBox(EntInfo& pent);
	void ESP();
	void DrawBox();
	void DrawHealth(C_BaseEntity* ent);
	void DrawHitboxes();
	RECT BBox;
	int YOffset = 0;
private:

}; extern Visuals g_Visuals;