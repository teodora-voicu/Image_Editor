#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>

int** load(int *c, int *l, char *type, char *filename)
{
    char a[100];
    scanf("%s",filename);
    FILE *file=fopen(filename,"r");
    if(file==NULL)
    {
        return NULL;
    }

    fgets(type,3,file);
    type[2]='\0';

    fscanf(file,"%d %d",c,l);
    fgets(a, 100, file);
    if(strcmp(type,"P2")==0 || strcmp(type,"P3")==0 || strcmp(type,"P5")==0 || strcmp(type,"P6")==0)
    fgets(a, 100, file);
    int **mat=(int **)malloc((*l)*sizeof(int *));
    if(mat==NULL)
    {
        return NULL;
    }

    if(strcmp(type,"P3")==0 || strcmp(type,"P6")==0) {
        (*c)=3*(*c);
    }

    for(int i=0; i<(*l); i++)
    {
        mat[i]=(int *)calloc(*c,sizeof(int));
        if(mat[i]==NULL)
        {
            return NULL;
        }
        for(int j=0; j<(*c); j++)
        {
            if(strcmp(type,"P1")==0 || strcmp(type,"P2")==0 || strcmp(type,"P3")==0) {
                fscanf(file,"%d ",&mat[i][j]);
            } else {
                fread(&mat[i][j],sizeof(unsigned char),1,file);
            }
        }
    }

    fclose(file);
    return mat;
    return 0;
}


void generalload(int ***img, int *c, int *l, char *type)
{
    char filename[100];
    if(*img!=NULL)
    {
        for(int i=0;i<(*l);i++)
        free((*img)[i]);
        free(*img);
        *img=NULL;
    }

    int **temp=load(c,l,type,filename);
    if(temp != NULL)
    {
        *img = temp;
        printf("Loaded %s\n",filename);
    }
    else
    printf("Failed to load %s\n",filename);
}


void freemem(int ***img, int *l)
{
    for(int i=0;i<(*l);i++)
        free((*img)[i]);
    free(*img);
    *img=NULL;
}




void saveascii(int ***img, int *c, int *l, char *type, char *filename)
{
    FILE* file=fopen(filename,"w");
    int **mat=*img;
    if(file==NULL)
    {
        return ;
    }
    if(strcmp(type,"P4")==0)
    fprintf(file,"P1\n");
    else if(strcmp(type,"P5")==0)
    fprintf(file,"P2\n");
    else if(strcmp(type,"P6")==0)
    fprintf(file,"P3\n");
    else fprintf(file,"%s\n",type);

    if (strcmp(type,"P6")==0 || strcmp(type,"P3")==0) fprintf(file,"%d %d\n",*c / 3,*l);
    else fprintf(file,"%d %d\n",*c,*l);
    fprintf(file,"255\n"); //posibil eroare
    for(int i=0;i<(*l);i++)
    {
        for(int j=0;j<(*c);j++)
        {
            fprintf(file,"%d ",mat[i][j]);
        }
        fprintf(file,"\n");
    }
    fclose(file);
}



void savebinary(int ***img, int *c, int *l, char *type, char *filename)
{
    FILE* file=fopen(filename,"wb");
    int **mat=*img;
    if(file==NULL)
    {
        return ;
    }
    if(strcmp(type,"P1")==0)
    fprintf(file,"P4\n");
    else if(strcmp(type,"P2")==0)
    fprintf(file,"P5\n");
    else if(strcmp(type,"P3")==0)
    fprintf(file,"P6\n");
    else fprintf(file,"%s\n",type);

    if (strcmp(type,"P6")==0 || strcmp(type,"P3")==0) fprintf(file,"%d %d\n",*c / 3,*l);
    else fprintf(file,"%d %d\n",*c,*l);

    fprintf(file,"255\n"); //posibil eroare
    for(int i=0;i<(*l);i++)
    {
        for(int j=0;j<(*c);j++)
        fwrite(&mat[i][j],sizeof(unsigned char),1,file);

    }
    fclose(file);
}



void save(int ***img,int *c, int *l, char *type)
{
    char filename[100];
    char param[100],p[100];
    fgets(param,100,stdin);
    strcpy(p,param+1);
    strcpy(param,p);
    param[strlen(param)-1]='\0';
    char *x=strtok(param," ");
    strcpy(filename,x);
    x=strtok(NULL," ");
    if(*img==NULL)
    printf("No image loaded\n");
    else
    {
        if(x!=NULL && strcmp(x,"ascii")==0)
            saveascii(img,c,l,type,filename);
        else
            savebinary(img,c,l,type,filename);
            
        printf("Saved %s\n",filename);
    }
}



void selectimg(int ***img, int *c, int *l, int *x1, int *y1, int *x2, int *y2, char *type)
{
    char param[100],tmp[100];
    fgets(param,100,stdin);
    strcpy(tmp,param+1);
    strcpy(param,tmp);
    while (param[strlen(param)-1] == ' ' || param[strlen(param)-1] == '\n')
        param[strlen(param)-1]='\0';

    int **mat=*img;
    if(mat==NULL) {
        printf("No image loaded\n");
        return;
    }
    if(strcmp(param,"ALL")==0) {
        (*x1)=0;
        (*y1)=0;
        (*x2)=(*c)-1;
        (*y2)=(*l)-1;
        printf("Selected ALL\n");
    } else {
        int cx1,cy1,cx2,cy2,aux;
        if(sscanf(param,"%d %d %d %d",&cx1,&cy1,&cx2,&cy2) < 4) {
            printf("Invalid command\n");
            return;
        }

        if(cx1>cx2)
        {
            aux=cx1;
            cx1=cx2;
            cx2=aux;
        }
        if(cy1>cy2)
        {
            aux=cy1;
            cy1=cy2;
            cy2=aux;
        }

        if (!strcmp(type, "P3") || !strcmp(type, "P6")) {
            cx1 *= 3;
            cx2 *= 3;
        }

        if(cx1<0 || cx1>(*c) || cy1<0 || cy1>(*l))
        {
            printf("Invalid set of coordinates\n");
            return;
        }
        if(cx2<0 || cx2>(*c) || cy2<0 || cy2>(*l))
        {
            printf("Invalid set of coordinates\n");
            return;
        }
        if(cx1==cx2 || cy1==cy2)
        {
            printf("Invalid set of coordinates\n");
            return;
        }
        *x1=cx1;
        *x2=cx2-1;
        *y1=cy1;
        *y2=cy2-1;

        // printf("%d %d %d %d %d %d\n", *c, *l, cx1,cy1,cx2,cy2);

        if (!strcmp(type, "P3") || !strcmp(type, "P6")) {
            printf("Selected %d %d %d %d\n",cx1 / 3,cy1,cx2 / 3,cy2);
        } else printf("Selected %d %d %d %d\n",cx1,cy1,cx2,cy2);
    }
}


void crop(int ***img, int *c, int *l, int *x1, int *y1, int *x2, int *y2)
{
    int **mat=*img;
    if(mat==NULL)
    {
        printf("No image loaded\n");
        return;
    }

    int c2=(*x2)-(*x1)+1;
    int l2=(*y2)-(*y1)+1;
    int i,j;
    int **img2=(int **)malloc(l2*sizeof(int*));
    if(img2==NULL)
    {
        return;
    }
    for(i=0;i<l2;i++)
    {
        img2[i]=(int *)calloc(c2,sizeof(int));
        for(j=0;j<c2;j++) {
           img2[i][j]=mat[i+(*y1)][j+(*x1)];
        }
    }
    for(int i=0;i<(*l);i++)
        free(mat[i]);
    free(mat);
    *img=img2;

    *c=c2;
    *l=l2;
    *x1=0;
    *y1=0;
    *y2=(*l)-1;
    *x2=(*c)-1;
    
    printf("Image cropped\n");
}

bool ispow2(int x)
{
    return (x & (x-1))==0;
}


void printhisto(long long *newf,int stars, int bins)
{
    int max=newf[0],i,nrstars,j;
    for(i=1;i<bins;i++)
    {
        if(newf[i]>max)
        max=newf[i];
    }
    for(i=0;i<bins;i++)
    {
        nrstars=(newf[i]*stars)/max;
        printf("%d\t|\t",nrstars);
        for(j=0;j<nrstars;j++)
            printf("*");
        printf("\n");
    }
}

void histogram(int ***img, int *c, int *l, char *type)
{
    int stars=1,bins=1;
    int tmp_p;
    char tmp[100];
    fgets(tmp,100,stdin);
    int aux=sscanf(tmp," %d %d %d",&stars,&bins, &tmp_p);
    int **mat=*img;

    if(mat==NULL)
    {
        printf("No image loaded\n");
        return;
    }
    if(aux!=2 || !ispow2(bins) || bins>256)
    {
        printf("Invalid command\n");
        return;
    }
    if (!strcmp(type, "P3") || !strcmp(type, "P6"))
    {
        printf("Black and white image needed\n");
        return;
    }
    
    int freq[256]={0};
    int i,j;
    for(i=0;i<(*l);i++)
    {
        for(j=0;j<(*c);j++)
        {
            freq[mat[i][j]]++;
        }
    }
    long long *newf=(long long *)calloc(bins,sizeof(long long));
    int val = 256 / bins;

    int cnt = 0;
    for(i=0;i<256;i++)
    {
        if(i && i % val == 0) {
            cnt++;
        }
        newf[cnt] = newf[cnt] + freq[i];
    }

    printhisto(newf,stars,bins);
    free(newf);
}

int clamp(int x, int min, int max)
{
    if(x<min)
    x=min;
    else if(x>max)
    x=max;
    return x;
}


void equalize(int ***img, int *c, int *l, char *type) {
    int **mat=*img;
    if(mat==NULL)
    {
        printf("No image loaded\n");
        return;
    }

    if (!strcmp(type, "P3") || !strcmp(type, "P6")) {
        printf("Black and white image needed\n");
        return;
    }

    int freq[256]={0};
    int i,j;
    for(i=0;i<(*l);i++)
    for(j=0;j<(*c);j++)
    {
        freq[mat[i][j]]++;
    }
    long long sfreq[256];
    sfreq[0]=freq[0];
    for(i=1;i<256;i++)
    sfreq[i]=sfreq[i-1]+freq[i];
    for(i=0;i<(*l);i++)
    for(j=0;j<(*c);j++)
    {
        double x=(double)255*sfreq[mat[i][j]]/((*c)*(*l));
        mat[i][j]=round(x);
        mat[i][j]=clamp(mat[i][j],0,255);
    }
     printf("Equalize done\n");
}

int multiply_m(int **mat, int c, int l, double A[][3]) {
    double S = 0;
    for (int i = l; i < l + 3; i++) {
        for (int j = c; j < c + 9; j = j + 3) {
            S = S + mat[i][j] * A[i - l][(j - c) / 3];
        }
    }

    return clamp(round(S), 0, 255);
}

void apply(int ***img, int *c, int *l, char *type, int x1, int y1, int x2, int y2) {
    char param[100], tmp[100];
    fgets(param, 100, stdin);
    strcpy(tmp, param + 1);
    strcpy(param, tmp);
    while (param[strlen(param)-1] == ' ' || param[strlen(param)-1] == '\n')
        param[strlen(param) - 1] = '\0';
    
    int **mat=*img;
    if(mat==NULL)
    {
        printf("No image loaded\n");
        return;
    }
    if (strlen(param) == 0) {
        printf("Invalid command\n");
        return;
    }
    if (strcmp(param, "EDGE") &&  strcmp(param, "SHARPEN") && strcmp(param, "BLUR") && strcmp(param, "GAUSSIAN_BLUR")) {
        printf("APPLY parameter invalid\n");
        return;
    }
    if (!strcmp(type, "P2") || !strcmp(type, "P5")) {
        printf("Easy, Charlie Chaplin\n");
        return;
    }

    double m[3][3];
    if (!strcmp(param, "EDGE")) {
        m[0][0] = -1;
        m[0][1] = -1;
        m[0][2] = -1;

        m[1][0] = -1;
        m[1][1] =  8;
        m[1][2] = -1;

        m[2][0] = -1;
        m[2][1] = -1;
        m[2][2] = -1;
    } else if (!strcmp(param, "SHARPEN")) {
        m[0][0] =  0;
        m[0][1] = -1;
        m[0][2] =  0;

        m[1][0] = -1;
        m[1][1] =  5;
        m[1][2] = -1;

        m[2][0] =  0;
        m[2][1] = -1;
        m[2][2] =  0;
    } else if (!strcmp(param, "BLUR")) {
        m[0][0] = 1.0/9;
        m[0][1] = 1.0/9;
        m[0][2] = 1.0/9;

        m[1][0] = 1.0/9;
        m[1][1] = 1.0/9;
        m[1][2] = 1.0/9;

        m[2][0] = 1.0/9;
        m[2][1] = 1.0/9;
        m[2][2] = 1.0/9;
    } else if (!strcmp(param, "GAUSSIAN_BLUR")) {
        m[0][0] = 1.0/16;
        m[0][1] =  1.0/8;
        m[0][2] = 1.0/16;

        m[1][0] = 1.0/8;
        m[1][1] = 1.0/4;
        m[1][2] = 1.0/8;

        m[2][0] = 1.0/16;
        m[2][1] =  1.0/8;
        m[2][2] = 1.0/16;
    }

    int c2=x2-x1+1;
    int l2=y2-y1+1;

    int **applied_img = (int**)malloc(l2 * sizeof(int*));
    for (int i = 0; i < l2; i++) {
        applied_img[i] = (int*)malloc(c2 * sizeof(int));
    }

    if (y1 == 0) {
        for (int i = 0; i < c2; i++) {
            applied_img[0][i] = mat[0][i];
        }
    }
    if (x1 == 0) {
        for (int w = 0; w < 3; w++) {
            for (int i = 0; i < l2; i++) {
                applied_img[i][w] = mat[i][w];
            }
        }
    }
    if (y2 == *l - 1) {
        for (int i = 0; i < c2; i++) {
            applied_img[l2 - 1][i] = mat[y2][i + x1];
        }
    }
    if (x2 == *c - 1) {
        for (int w = 0; w < 3; w++) {
            for (int i = 0; i < l2; i++) {
                applied_img[i][c2 - w - 1] = mat[i + y1][x2 - w];
            }
        }
    }

    for (int i = y1; i <= y2; i++) {
        if (i == 0 || i == *l - 1) continue;
        for (int j = x1; j <= x2; j++) {
            if (j < 3 || j > *c - 4) continue;
            applied_img[i - y1][j - x1] = multiply_m(mat, j - 3, i - 1, m);
        }
    }

    for (int i = y1; i <= y2; i++) {
        for (int j = x1; j <= x2; j++) {
            mat[i][j] = applied_img[i - y1][j - x1];
        }
    }

    for (int i = 0; i < l2; i++) free(applied_img[i]);
    free(applied_img);

    printf("APPLY %s done\n", param);
}

int main()
{
    int **mat=NULL;
    int c=0,l=0;
    char type[3];
    int x1,y1,x2,y2;
    char command[15];
    while(1)
    {
        scanf("%s",command);
        if(strcmp(command,"LOAD")==0)
        {
            generalload(&mat,&c,&l,type);
            x1 = 0; y1 = 0; x2 = c - 1; y2 = l - 1;  
        } else if(strcmp(command,"SAVE")==0) {
            save(&mat,&c,&l,type);
        } else if(strcmp(command,"SELECT")==0) {
            selectimg(&mat,&c,&l,&x1,&y1,&x2,&y2, type);
        } else if(strcmp(command,"CROP")==0) {
            crop(&mat,&c,&l,&x1,&y1,&x2,&y2);
        } else if(strcmp(command,"HISTOGRAM")==0) {
            histogram(&mat,&c,&l,type);
        } else if(strcmp(command,"EQUALIZE")==0) {
            equalize(&mat,&c,&l,type);
        } else if(strcmp(command,"APPLY")==0) {
            apply(&mat,&c,&l,type, x1, y1, x2, y2);
        } else if(strcmp(command,"EXIT")==0) {
            if (!mat) {
                printf("No image loaded\n");
            }
            break;
        } else {
            char tmp[100];
            fgets(tmp, 100, stdin);
            printf("Invalid command\n");
        }
        
    }

    if (mat) {
        freemem(&mat,&l);
    }
    return 0;
}



