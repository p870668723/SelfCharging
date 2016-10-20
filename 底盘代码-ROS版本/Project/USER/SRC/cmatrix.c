#include "cmatrix.h"

//���󷽷�
//����m  
void matrix_setRows(struct _Matrix *m,int mm)
{
	m->Rows = mm; 
}

//����n  
void matrix_setCols(struct _Matrix *m,int nn)
{
	m->Cols = nn;
}

//��ʼ��  
void matrix_init(struct _Matrix *Mat,int nRows,int nCols)
{
	Mat->Rows = nRows;
	Mat->Cols = nCols;
	Mat->arr = (float *)malloc(nRows * nCols * sizeof(float));
}


//�ͷ�  
void matrix_free(struct _Matrix *m)
{
	free(m->arr);
}

//��ȡi,j���������  
//ʧ�ܷ���-31415,�ɹ�����ֵ  
float matrix_read(struct _Matrix *m,int i,int j)
{
	if (i >= m->Rows || j >= m->Cols)
    {
        return -31415;
    }
    
    return *(m->arr + i * m->Cols + j);
}

//д��i,j���������  
//ʧ�ܷ���-1,�ɹ�����1  
int matrix_write(struct _Matrix *m,int i,int j,float val)
{
	if (i >= m->Rows || j >= m->Cols)
    {
        return -1;
    }
      
    *(m->arr + i * m->Cols + j) = val;  
    return 1;  
}

//��������
//�ɹ�����1,ʧ�ܷ���-1  
int matrix_add(struct _Matrix *A,struct _Matrix *B,struct _Matrix *C)
{
	int i = 0;  
    int j = 0;  
      
    //�ж��Ƿ��������  
	if (A->Rows != B->Rows || A->Cols != B->Cols || \
        A->Rows != C->Rows || A->Cols != C->Cols)  
    {  
        return -1;  
    }  
    //����  
    for (i = 0;i < C->Rows;i++)  
    {  
        for (j = 0;j < C->Cols;j++)  
        {  
            matrix_write(C,i,j,matrix_read(A,i,j) + matrix_read(B,i,j));  
        }  
    }  
      
    return 1; 
}

//C = A - B  
//�ɹ�����1,ʧ�ܷ���-1  
int matrix_subtract(struct _Matrix *A,struct _Matrix *B,struct _Matrix *C)
{
	int i = 0;  
    int j = 0;  
      
    //�ж��Ƿ��������  
    if (A->Rows != B->Rows || A->Cols != B->Cols || \
        A->Rows != C->Rows || A->Cols != C->Cols)  
    {  
        return -1;  
    }  
    //����  
    for (i = 0;i < C->Rows;i++)  
    {  
        for (j = 0;j < C->Cols;j++)  
        {  
            matrix_write(C,i,j,matrix_read(A,i,j) - matrix_read(B,i,j));  
        }  
    }  
      
    return 1; 
}

//C = A * B  
//�ɹ�����1,ʧ�ܷ���-1  
int matrix_multiply(struct _Matrix *A,struct _Matrix *B,struct _Matrix *C)
{
	int i = 0;  
    int j = 0;  
    int k = 0;  
    float temp = 0;  
      
    //�ж��Ƿ��������  
    if (A->Rows != C->Rows || B->Cols != C->Cols || \
        A->Cols != B->Rows)  
    {  
        return -1;  
    }  
    //����  
    for (i = 0;i < C->Rows;i++)  
    {  
        for (j = 0;j < C->Cols;j++)  
        {  
            temp = 0;  
            for (k = 0;k < A->Cols;k++)  
            {  
                temp += matrix_read(A,i,k) * matrix_read(B,k,j);  
            }  
            matrix_write(C,i,j,temp);  
        }  
    }  
      
    return 1; 
}

//����ʽ��ֵ,ֻ�ܼ���2 * 2,3 * 3  
//ʧ�ܷ���-31415,�ɹ�����ֵ  
float matrix_det(struct _Matrix *A)
{
	float value = 0;  
      
    //�ж��Ƿ��������  
    if (A->Rows != A->Cols || (A->Rows != 2 && A->Rows != 3))  
    {  
        return -31415;  
    }  
    //����  
    if (A->Rows == 2)  
    {  
        value = matrix_read(A,0,0) * matrix_read(A,1,1) - matrix_read(A,0,1) * matrix_read(A,1,0);  
    }  
    else  
    {  
        value = matrix_read(A,0,0) * matrix_read(A,1,1) * matrix_read(A,2,2) + \
                matrix_read(A,0,1) * matrix_read(A,1,2) * matrix_read(A,2,0) + \
                matrix_read(A,0,2) * matrix_read(A,1,0) * matrix_read(A,2,1) - \
                matrix_read(A,0,0) * matrix_read(A,1,2) * matrix_read(A,2,1) - \
                matrix_read(A,0,1) * matrix_read(A,1,0) * matrix_read(A,2,2) - \
                matrix_read(A,0,2) * matrix_read(A,1,1) * matrix_read(A,2,0);  
    }  
      
    return value; 
}

//��ת�þ���,B = AT  
//�ɹ�����1,ʧ�ܷ���-1  
int matrix_transpos(struct _Matrix *A,struct _Matrix *B)
{
	int i = 0;  
    int j = 0;  
      
    //�ж��Ƿ��������  
    if (A->Rows != B->Cols || A->Cols != B->Rows)  
    {  
        return -1;  
    }  
    //����  
    for (i = 0;i < B->Rows;i++)  
    {  
        for (j = 0;j < B->Cols;j++)  
        {  
            matrix_write(B,i,j,matrix_read(A,j,i));  
        }  
    }  
      
    return 1;  
}

//�������,B = A^(-1)  
//�ɹ�����1,ʧ�ܷ���-1  
int matrix_inverse(struct _Matrix *A,struct _Matrix *B)
{
	int i = 0;  
    int j = 0;  
    int k = 0;  
    struct _Matrix m;  
    float temp = 0;  
    float b = 0;  
      
    //�ж��Ƿ��������  
    if (A->Rows != A->Cols || B->Rows != B->Cols || A->Rows != B->Rows)  
    {  
        return -1;  
    }  
      
    /* 
    //�����2ά����3ά������ʽ�ж��Ƿ���� 
    if (A->m == 2 || A->m == 3) 
    { 
        if (det(A) == 0) 
        { 
            return -1; 
        } 
    } 
    */  
      
    //�������m = A | B��ʼ��   
//	matrix_setRows(&m,A->Rows);
//	matrix_setCols(&m,2 * A->Cols);
	matrix_init(&m,A->Rows,2*A->Cols);
    for (i = 0;i < m.Rows;i++)  
    {  
        for (j = 0;j < m.Cols;j++)  
        {  
            if (j <= A->Cols - 1)  
            {  
                matrix_write(&m,i,j,matrix_read(A,i,j));  
            }  
            else  
            {  
                if (i == j - A->Cols)  
                {  
                    matrix_write(&m,i,j,1);  
                }  
                else  
                {  
                    matrix_write(&m,i,j,0);  
                }  
            }  
        }  
    }  
      
    //��˹��Ԫ  
    //�任������  
    for (k = 0;k < m.Rows - 1;k++)  
    {  
        //�������Ϊk,k����Ϊ0,���б任  
        if (matrix_read(&m,k,k) == 0)  
        {  
            for (i = k + 1;i < m.Rows;i++)  
            {  
                if (matrix_read(&m,i,k) != 0)  
                {  
                    break;  
                }  
            }  
            if (i >= m.Rows)  
            {  
                return -1;  
            }  
            else  
            {  
                //������  
                for (j = 0;j < m.Cols;j++)  
                {  
                    temp = matrix_read(&m,k,j);  
                    matrix_write(&m,k,j,matrix_read(&m,k + 1,j));  
                    matrix_write(&m,k + 1,j,temp);  
                }  
            }  
        }  
          
        //��Ԫ  
        for (i = k + 1;i < m.Rows;i++)  
        {  
            //��ñ���  
            b = matrix_read(&m,i,k) / matrix_read(&m,k,k);  
            //�б任  
            for (j = 0;j < m.Cols;j++)  
            {  
                temp = matrix_read(&m,i,j) - b * matrix_read(&m,k,j);  
                matrix_write(&m,i,j,temp);  
            }  
        }  
    }  
    //�任������  
    for (k = m.Rows - 1;k > 0;k--)  
    {  
        //�������Ϊk,k����Ϊ0,���б任  
        if (matrix_read(&m,k,k) == 0)  
        {  
            for (i = k + 1;i < m.Cols;i++)  
            {  
                if (matrix_read(&m,i,k) != 0)  
                {  
                    break;  
                }  
            }  
            if (i >= m.Rows)  
            {  
                return -1;  
            }  
            else  
            {  
                //������  
                for (j = 0;j < m.Cols;j++)  
                {  
                    temp = matrix_read(&m,k,j);  
                    matrix_write(&m,k,j,matrix_read(&m,k + 1,j));  
                    matrix_write(&m,k + 1,j,temp);  
                }  
            }  
        }  
          
        //��Ԫ  
        for (i = k - 1;i >= 0;i--)  
        {  
            //��ñ���  
            b = matrix_read(&m,i,k) / matrix_read(&m,k,k);  
            //�б任  
            for (j = 0;j < m.Cols;j++)  
            {  
                temp = matrix_read(&m,i,j) - b * matrix_read(&m,k,j);  
                matrix_write(&m,i,j,temp);  
            }  
        }  
    }  
    //����߷���Ϊ��λ����  
    for (i = 0;i < m.Rows;i++)  
    {  
        if (matrix_read(&m,i,i) != 1)  
        {  
            //��ñ���  
            b = 1 / matrix_read(&m,i,i);  
            //�б任  
            for (j = 0;j < m.Cols;j++)  
            {  
                temp = matrix_read(&m,i,j) * b;  
                matrix_write(&m,i,j,temp);  
            }  
        }  
    }  
    //��������  
    for (i = 0;i < B->Rows;i++)  
    {  
        for (j = 0;j < B->Rows;j++)  
        {  
            matrix_write(B,i,j,matrix_read(&m,i,j + m.Rows));  
        }  
    }  
    //�ͷ��������  
    matrix_free(&m);  
      
    return 1; 
}

