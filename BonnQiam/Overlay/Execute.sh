./Test /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/s/sM1_grid.txt /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/s/sM2_grid.txt sM12_overlay.txt | tee -a sM12_overlay.log

./Test /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/s/sM2_grid.txt /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/s/sM3_grid.txt sM23_overlay.txt | tee -a sM23_overlay.log

./Test /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/b/bM1_grid.txt /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/b/bM2_grid.txt bM12_overlay.txt | tee -a bM12_overlay.log

./Test /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/b/bM2_grid.txt /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/b/bM3_grid.txt bM23_overlay.txt | tee -a bM23_overlay.log

./Test /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/m/mM1_grid.txt /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/m/mM2_grid.txt mM12_overlay.txt | tee -a mM12_overlay.log

./Test /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/b/mM2_grid.txt /home/qiamking/Github/ICCAD_contest_2018_problem_c/BonnQiam/Grid/log/m/mM3_grid.txt mM23_overlay.txt | tee -a mM23_overlay.log