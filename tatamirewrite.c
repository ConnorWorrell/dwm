void 
tatamiReWrite(Monitor *m) {
	unsigned int i, n, nx, ny, nw, nh,
				 mats, tc,
				 tnx, tny, tnw, tnh = -1, windowNumber, windowCount;

	// stack side weighting
	float Adj1 = 1, Adj2T = 1, Adj2B = 1, Adj3TL = 1, Adj3TR = 1, Adj3B = 1, Adj4T = 1, Adj4B = 1, 
	//		 Adj4R = 1, Adj4L = 1, Adj5TL = 0.8571428571f, Adj5TR = 0.4285714286f, Adj5BL = 0.4285714286f, Adj5BR = 0.8571428571f, Adj5C = 0.4285714286f;
			 Adj4R = 1, Adj4L = 1, Adj5TL = 0.6666666f, Adj5TR = 0.6666666f, Adj5BL = 0.6666666f, Adj5BR = 0.6666666f, Adj5C = 0.333333f;

	Client *c;

	for (windowNumber = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), windowNumber++) {
	}
	tc = windowNumber-m->nmaster; // number of windows in stack
	mats = tc/5; // number of full mats

	float mfacts = 0, sfacts = 0, sfactsAdjusted = -1;
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (n < m->nmaster)
			mfacts += c->cfact; // Total master height squish factor
		else {
			windowCount = n < windowNumber - mats * 5 ? tc - (mats * 5) : 5;
			i = n - m->nmaster < tc - mats*5 ? n - m->nmaster : n - m->nmaster - (tc - mats * 5);
			switch (windowCount){ //
				case 1: // Full 
					sfactsAdjusted = Adj1;
					break;
				case 2:
					if((i % 5) == 0) sfactsAdjusted = Adj2T; //up
					else if((i % 5) == 1) sfactsAdjusted = Adj2B; //down
					break;
				case 3://bottom, up-left and up-right
					if((i % 5) == 0) sfactsAdjusted = Adj3TL; //up-left
					else if((i % 5) == 1) sfactsAdjusted = Adj3TR;//up-right
					else if((i % 5) == 2) sfactsAdjusted = Adj3B;//bottom
					break;
				case 4://bottom, left, right and top
					if((i % 5) == 0) sfactsAdjusted = Adj4T; //top
					else if((i % 5) == 1) sfactsAdjusted = Adj4L;//left
					else if((i % 5) == 2) sfactsAdjusted = Adj4R;//right
					else if((i % 5) == 3) sfactsAdjusted = Adj4B;//bottom
					break;
				case 5:
					if((i % 5) == 0) sfactsAdjusted = Adj5TL; // top left
					else if((i % 5) == 1) sfactsAdjusted = Adj5TR;// top right
					else if((i % 5) == 2) sfactsAdjusted = Adj5C;// center
					else if((i % 5) == 3) sfactsAdjusted = Adj5BR;// bottom right
					else if((i % 5) == 4) sfactsAdjusted = Adj5BL;// bottom left
					break; 
					
			}
			sfacts += c->cfact * sfactsAdjusted; // Total stack hight squish factor
		}
	}


	if(n == 0) // No windows
		return;
	
	nx = m->wx;
	ny = 0;
	nw = m->ww;
	nh = m->wh;

	c = nexttiled(m->clients);
	
	//Master				
	if (n > m->nmaster)
		nw = m->nmaster ? m->ww * m->mfact : 0; // Master windows
	else
		nw = m->ww - m->gappx; // No master windows

	// Place master windows
	int h;
	for (i = 0; c && i < m->nmaster; c = nexttiled(c->next), i++) {
		h = (m->wh - ny - m->gappx) * (c->cfact / mfacts) - m->gappx;
		resize(c, m->wx + m->gappx, m->wy + ny + m->gappx, nw - (2*c->bw) - m->gappx, h - (2*c->bw), 0);
		if (ny + HEIGHT(c) + 2 * m->gappx < m->wh)
			ny += HEIGHT(c) + m->gappx;
		mfacts -=c->cfact;
	}
	
	
	nx += nw; // Move nx to stack side
	nw = m->ww - nw; // Change nw to stack width
	ny = 0;

	Bool Bottom = False;
	

	if(n>m->nmaster) // If there are stack windows
	{
		
		tc = n-m->nmaster; // number of windows in stack
		mats = tc/5; // number of full mats

		ny = m->wy + ny + m->gappx;

		Bottom = 0;
		
		// less than 5 mats mat
		for(i = 0; c && (i < (tc % 5)); c = nexttiled(c->next))
		{
			tnw=nw;
			tnx=nx;
			tny=ny;

			switch(tc - (mats*5))
					{
						case 1://fill
							tnh=nh*c->cfact/sfacts;
							tnw-=2*m->gappx;
							tnh-=2*m->gappx;
							tnx+=m->gappx;
							ny+= tnh + m -> gappx;
							Bottom = 1;
							break;
						case 2://up and down
							//tnh=nh*c->cfact/sfacts;
							tnh=m->wh*c->cfact/sfacts;
							if((i % 5) == 0) //up
							{
								tny = ny;
								tnx+=m->gappx;
								tnh*=Adj2T;
								tnh-=3*m->gappx/2;
								tnw-=2*m->gappx;
								ny+= tnh + m -> gappx;
							}
							else if((i % 5) == 1) //down
							{
								tny = ny;
								tnh*=Adj2B;
								tnh-=3*m->gappx/2;
								tnw-=2*m->gappx;
								tnx+=m->gappx;
								ny+= tnh + m -> gappx;
								Bottom = 1;
							}
							break;
						case 3://bottom, up-left and up-right
							if((i % 5) == 0) //up-left
							{
								tnh=nh*(c->cfact+nexttiled(c->next)->cfact)/(2*sfacts);
								tnh*=Adj3TL*2;
								tnw = tnw/2;
								tnx+=m->gappx;
								tnw-=(3*m->gappx)/2;
								tnh-=(3*m->gappx)/2;
							}
							else if((i % 5) == 1)//up-right
							{
								tnx += tnw/2+m->gappx/2;
								tnw = tnw/2 - (3*m->gappx)/2;
								ny+= tnh + m -> gappx;
							}
							else if((i % 5) == 2)//bottom
							{
								tnh=nh*c->cfact/sfacts;
								tnh -= (3*m->gappx)/2;
								tny = ny;
								tnx+=m->gappx;
								tnw-=m->gappx*2;
								ny+= tnh + m -> gappx;
								Bottom = 1;
							}
							break;
						case 4://bottom, left, right and top
							if((i % 5) == 0) //top
							{
								tnh=nh*c->cfact/sfacts;
								tnh -=(3*m->gappx)/2;
								tnx+=m->gappx;
								tnw-=2*m->gappx;
								ny+= tnh + m -> gappx;
							}
							else if((i % 5) == 1)//left
							{
								tnh=nh*(c->cfact+nexttiled(c->next)->cfact)/(2*sfacts);
								tnh *= 2;
								tnw = tnw/2-(3*m->gappx)/2;
								tny = ny;
								tnh -= m->gappx;
								tnx+=m->gappx;
							}
							else if((i % 5) == 2)//right
							{
								tnx += tnw/2+m->gappx/2;
								tnw = tnw/2-(3*m->gappx)/2;
								tny = ny;
								ny+= tnh + m -> gappx;
							}
							else if((i % 5) == 3)//bottom
							{
								tnh=nh*c->cfact/sfacts;
								tny = ny;
								tnh -= (3*m->gappx)/2;
								tnx+=m->gappx;
								tnw-=2*m->gappx;
								ny+= tnh + m -> gappx;
								Bottom = 1;
							}
							break;
					}
			++i;

			// Remove 1-2 pixle rounding error stackup
			if (mats <= 0 && Bottom){
				tnh = nh-tny-m->gappx + m->wy;
			}

			resize(c, tnx, tny, tnw - 2 * c->bw, tnh - 2 * c->bw, False);
		}
		
		//Complete	
		int centernh = -1;
		int Height1 = -1, Height2 = -1, Height3 = -1;

		for(i = 0; c && (mats>0); c = nexttiled(c->next)) {

			Bottom = 0;
				
			tnw=nw;
			tnx=nx;
			tny=ny;

			float TLC, TRC, CC, BRC, BLC;

			switch(i % 5)
			{
				case 0: //top-left-vert
					TLC = c->cfact * Adj5TL;
					TRC = nexttiled(c->next)->cfact * Adj5TR;
					CC = nexttiled(nexttiled(c->next)->next)->cfact * Adj5C;
					BRC = nexttiled(nexttiled(nexttiled(c->next)->next)->next)->cfact * Adj5BR;
					BLC = nexttiled(nexttiled(nexttiled(nexttiled(c->next)->next)->next)->next)->cfact * Adj5BL;
					Height1 = nh*(TRC+TLC/2)/sfacts;
					Height2 = nh*(CC+TLC/2+BRC/2)/sfacts;
					Height3 = nh*(BLC+BRC/2)/sfacts;
					
					tnh = Height1+Height2;
					tnh -= 3*m->gappx/2;
					tnw = (tnw)/3 - (3*m->gappx)/2;
					tnx+=m->gappx;
					tny = ny;
					break;
				case 1: //top-right-hor
					tnh = Height1;
					tnh -= 3*m->gappx/2;
					tnx += (tnw)/3+m->gappx/2;
					tnw = (tnw*2)/3-(3*m->gappx/2);
					tny = ny;
					ny += tnh + m->gappx;
					break;
				case 2: //center
					tnh = Height2;
					tnh -= m->gappx;
					tnx += (tnw)/3+m->gappx/2;
					tnw = (tnw)/3-m->gappx;
					tny = ny;
					centernh = tnh;
					break;
				case 3: //bottom-right-vert
					tnh = Height2 + Height3;
					tnh -= m->gappx;
					tnx += (tnw*2)/3+m->gappx/2;
					tnw = (tnw)/3-(3*m->gappx)/2;
					tny = ny;
					ny += centernh + m->gappx;
					Bottom = 1;
					break;
				case 4: //(oldest) bottom-left-hor
					tnh = Height3;
					tnh -= m->gappx;
					tnw = (2*tnw)/3-(3*m->gappx)/2;
					tny = ny;
					tnx+=m->gappx;
					ny += tnh + m->gappx;
					Bottom = 1;
					break;
				default:
					break;
			}
			
			// Handle pixel rounding error stackup
			if (i/5 == mats-1 && Bottom){
				tnh = nh-tny-m->gappx + m->wy;
			}
			
			++i;

			resize(c, tnx, tny, tnw - 2 * c->bw, tnh - 2 * c->bw, False);
		}
	}
}
