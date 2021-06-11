void 
tatami(Monitor *m) {
	unsigned int i, n, nx, ny, nw, nh,
				 mats, tc,
				 tnx, tny, tnw, tnh;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), ++n);
	if(n == 0)
		return;
	
	nx = m->wx;
	ny = 0;
	nw = m->ww;
	nh = m->wh;

	c = nexttiled(m->clients);
	
	if(n != 1)  nw = m->ww * m->mfact;
				ny = m->wy;
	//Master				
	resize(c, nx + gappx, ny + gappx, nw - 2 * c->bw - (n==1?2:1)*gappx, nh - 2 * c->bw - 2 * gappx, False);
	
	c = nexttiled(c->next);
	
	nx += nw;
	nw = m->ww - nw;
	
	if(n>1)
	{
	
	tc = n-1;
	mats = tc/5;
	
	nh/=(mats + (tc % 5 > 0));

	//Noncomplete	
	for(i = 0; c && (i < (tc % 5)); c = nexttiled(c->next))
	{
		tnw=nw;
		tnx=nx;
		tnh=nh;
		tny=ny;

		if(mats >= 1){
			tnh+=gappx/2;
		}

		switch(tc - (mats*5))
				{
					case 1://fill
						tnw-=2*gappx;
						tnh-=2*gappx;
						tnx+=gappx;
						tny+=gappx;
						break;
					case 2://up and down
						if((i % 5) == 0) //up
						{
							tny+=gappx;
							tnx+=gappx;
							tnh/=2;
							tnh-=(gappx*3)/2;
							tnw-=2*gappx;
						}
						else if((i % 5) == 1) //down
						{
							int tnytmp = tny;
							tny = ny+2*gappx+nh/2-(gappx*3)/2;
							tnh=tnh-(tny-tnytmp)-gappx;
							tnw-=2*gappx;
							tnx+=gappx;
						}
						break;
					case 3://bottom, up-left and up-right
						if((i % 5) == 0) //up-left
						{
							tnw = tnw/2;
							tnh = (2*tnh)/3;
							tnx+=gappx;
							tny+=gappx;
							tnw-=(3*gappx)/2;
							tnh-=(3*gappx)/2;
						}
						else if((i % 5) == 1)//up-right
						{
							tnx += tnw/2+gappx/2;
							tnw = tnw/2 - (3*gappx)/2;
							tnh = (2*tnh)/3 - (3*gappx)/2;
							tny+=gappx;
						}
						else if((i % 5) == 2)//bottom
						{
							int tnytemp=tny;
							tny += (2*tnh)/3 + gappx/2;	
							tnh=tnh-(tny-(2*tnytemp)/3-(3*gappx)/2)-gappx;
							tnx+=gappx;
							tnw-=gappx*2;
						}
						break;
					case 4://bottom, left, right and top
						if((i % 5) == 0) //top
						{
							tnh = (tnh)/4 - (3*gappx)/2;
							tny+=gappx;
							tnx+=gappx;
							tnw-=2*gappx;
						}
						else if((i % 5) == 1)//left
						{
							tnw = tnw/2-(3*gappx)/2;
							tny += tnh/4+gappx/2;
							tnh = (tnh)/2-gappx;
							tnx+=gappx;
						}
						else if((i % 5) == 2)//right
						{
							tnx += tnw/2+gappx/2;
							tnw = tnw/2-(3*gappx)/2;
							tny += tnh/4+gappx/2;
							tnh = (tnh)/2-gappx;
						}
						else if((i % 5) == 3)//bottom
						{
							int tnytmp = tny;
							tny += (3*tnh)/4+gappx/2;
							tnh=tnh-(tny-tnytmp)-gappx;
							tnx+=gappx;
							tnw-=2*gappx;
						}
						break;
				}
		++i;
		resize(c, tnx, tny, tnw - 2 * c->bw, tnh - 2 * c->bw, False);
	}
	
	++mats;

	//Complete	
	for(i = 0; c && (mats>0); c = nexttiled(c->next)) {

			if((i%5)==0)
			{
			--mats;
			if(((tc % 5) > 0)||(i>=5))
			ny+=nh;
			}
			
			tnw=nw;
			tnx=nx;
			tnh=nh;
			tny=ny;

			if(mats > 1 && mats == (tc / 5) && tc % 5 == 0){
				tnh+=gappx/2;
			} else if(mats > 1){
				tny-=gappx/2;
				tnh+=gappx;
			} else if(mats==1 && tc > 5){
				tny-=gappx/2;
				tnh+=gappx/2;
			}

			switch(i % 5)
			{
				case 0: //top-left-vert
					tnw = (tnw)/3 - (3*gappx)/2;
					tnh = (tnh*2)/3 - (3*gappx)/2;
					tnx+=gappx;
					tny+=gappx;
					break;
				case 1: //top-right-hor
					tnx += (tnw)/3+gappx/2;
					tnw = (tnw*2)/3-(3*gappx/2);
					tnh = (tnh)/3 - (3*gappx)/2;
					tny+=gappx;
					break;
				case 2: //center
					tnx += (tnw)/3+gappx/2;
					tnw = (tnw)/3-gappx;
					int tnytmp = tny;
					tny += (tnh)/3+gappx/2;
					tnh = (tnh*2)/3-(3*gappx)/2+(tnytmp+gappx-tny);
					//tnh = (tnh)/3-gappx;
					break;
				case 3: //bottom-right-vert
					tnx += (tnw*2)/3+gappx/2;
					tnw = (tnw)/3-(3*gappx)/2;
					tny += (tnh)/3+gappx/2;
					tnh = (tnh*2)/3-(3*gappx)/2;
					break;
				case 4: //(oldest) bottom-left-hor
					tnw = (2*tnw)/3-(3*gappx)/2;
					tny += (2*tnh)/3+gappx/2;
					tnh = tnh-(tnh*2)/3-(3*gappx)/2;
					tnx+=gappx;
					break;
				default:
					break;
			}
			
			++i;
			//i%=5;
		resize(c, tnx, tny, tnw - 2 * c->bw, tnh - 2 * c->bw, False);
		}
	}
}
