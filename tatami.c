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
	resize(c, nx + m->gappx, ny + m->gappx, nw - 2 * c->bw - (n==1?2:1)*m->gappx, nh - 2 * c->bw - 2 * m->gappx, False);
	
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
			tnh+=m->gappx/2;
		}

		int tnytmp = tny;

		switch(tc - (mats*5))
				{
					case 1://fill
						tnw-=2*m->gappx;
						tnh-=2*m->gappx;
						tnx+=m->gappx;
						tny+=m->gappx;
						break;
					case 2://up and down
						if((i % 5) == 0) //up
						{
							tny+=m->gappx;
							tnx+=m->gappx;
							tnh/=2;
							tnh-=(m->gappx*3)/2;
							tnw-=2*m->gappx;
						}
						else if((i % 5) == 1) //down
						{
							tny = ny+2*m->gappx+nh/2-(m->gappx*3)/2;
							tnh=tnh-(tny-tnytmp)-m->gappx;
							tnw-=2*m->gappx;
							tnx+=m->gappx;
						}
						break;
					case 3://bottom, up-left and up-right
						if((i % 5) == 0) //up-left
						{
							tnw = tnw/2;
							tnh = (2*tnh)/3;
							tnx+=m->gappx;
							tny+=m->gappx;
							tnw-=(3*m->gappx)/2;
							tnh-=(3*m->gappx)/2;
						}
						else if((i % 5) == 1)//up-right
						{
							tnx += tnw/2+m->gappx/2;
							tnw = tnw/2 - (3*m->gappx)/2;
							tnh = (2*tnh)/3 - (3*m->gappx)/2;
							tny+=m->gappx;
						}
						else if((i % 5) == 2)//bottom
						{
							tny += (2*tnh)/3 + m->gappx/2;	
							tnh=tnh-(tny - tnytmp)-m->gappx;
							tnx+=m->gappx;
							tnw-=m->gappx*2;
						}
						break;
					case 4://bottom, left, right and top
						if((i % 5) == 0) //top
						{
							tnh = (tnh)/4 - (3*m->gappx)/2;
							tny+=m->gappx;
							tnx+=m->gappx;
							tnw-=2*m->gappx;
						}
						else if((i % 5) == 1)//left
						{
							tnw = tnw/2-(3*m->gappx)/2;
							tny += tnh/4+m->gappx/2;
							tnh = (tnh)/2-m->gappx;
							tnx+=m->gappx;
						}
						else if((i % 5) == 2)//right
						{
							tnx += tnw/2+m->gappx/2;
							tnw = tnw/2-(3*m->gappx)/2;
							tny += tnh/4+m->gappx/2;
							tnh = (tnh)/2-m->gappx;
						}
						else if((i % 5) == 3)//bottom
						{
							tny += (3*tnh)/4+m->gappx/2;
							tnh=tnh-(tny-tnytmp)-m->gappx;
							tnx+=m->gappx;
							tnw-=2*m->gappx;
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
				tnh+=m->gappx/2;
			} else if(mats > 1){
				tny-=m->gappx/2;
				tnh+=m->gappx;
			} else if(mats==1 && tc > 5){
				tny-=m->gappx/2;
				tnh+=m->gappx/2;
			}

			int tnytmp = tny;

			switch(i % 5)
			{
				case 0: //top-left-vert
					tnw = (tnw)/3 - (3*m->gappx)/2;
					tnh = (tnh*2)/3 - (3*m->gappx)/2;
					tnx+=m->gappx;
					tny+=m->gappx;
					break;
				case 1: //top-right-hor
					tnx += (tnw)/3+m->gappx/2;
					tnw = (tnw*2)/3-(3*m->gappx/2);
					tnh = (tnh)/3 - (3*m->gappx)/2;
					tny+=m->gappx;
					break;
				case 2: //center
					tnx += (tnw)/3+m->gappx/2;
					tnw = (tnw)/3-m->gappx;
					tny += (tnh)/3+m->gappx/2;
					tnh = (tnh*2)/3-(3*m->gappx)/2+(tnytmp+m->gappx-tny);
					break;
				case 3: //bottom-right-vert
					tnx += (tnw*2)/3+m->gappx/2;
					tnw = (tnw)/3-(3*m->gappx)/2;
					tny += (tnh)/3+m->gappx/2;
					tnh = tnh-(tny-tnytmp)-m->gappx;
					break;
				case 4: //(oldest) bottom-left-hor
					tnw = (2*tnw)/3-(3*m->gappx)/2;
					tny += (2*tnh)/3+m->gappx/2;
					tnh = tnh-(tny-tnytmp)-m->gappx;
					tnx+=m->gappx;
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
