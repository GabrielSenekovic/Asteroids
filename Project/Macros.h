#pragma once

#define EqSgn(i,j) signbit(i) == signbit(j) //Equal Sign
#define Color(color) glColor3ub(color[0], color[1], color[2])

//temp = MMath::MatrixMultiplication(temp, MMath::GetRotationMatrix(1, time), 1, 3); makes me super nauseous
