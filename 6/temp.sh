show()
{
	# 先把变量引进，方便后续使用，分别为SOURCE（文本数组），INSERT（插入数组），NUMBER(插入位置，从1开始计数),COUNT(计数变量，用于插入),COUNT2(计数变量，用于提示当前字符串所在位置).INSERTLENGTH(插入数组长度)
	SOURCE=$1
	INSERT=$2
	NUMBER=$3
	COUNT=0
	COUNT2=1
	INSERTLENGTH=${#INSERT[*]}
	echo $NUMBER
	  
	# 判断一下，如果插入位置为缺省，就将插入位置定义为-1,跳过插入
	if ( test ! $INSERT )
	then
		((NUMBER=-1))
	fi

	# 开始输出SOURCE 文本数组中的内容，用for的迭代
	for((a=0;a<=LENGTH;a++))
	do
		# 这里通过COUNT判断一下是否到了输入位置，如果是，就录入插入数组
		if test $[COUNT] -eq $[NUMBER]
		then
			# 这里也是用for迭代输出插入数组
			for((b=1;b<=INSERTLENGTH;b++))
			do
				echo "$COUNT2 ${INSERT[$b]}"
				# 每一次输出都要自增
				((COUNT2++))
			done
		fi

		# 这里就是正常的录输出文本数组了
		echo "$COUNT2 ${TEXT[$a]}"
		# 每一次输出都要自增
		((COUNT2++))
	done
}

# 这是一个插入录入模式，是从第n行开始插入录入

eco()
{
	echo $1
	echo $2
	echo $3
}
insertSpecial()
{
	# 首先把NUMBER变量读入
	INSERTNUMBER=$1
	# 建立插入临时字符串
	INSERTCOUNT=1
	
	# 进入while true循环持续IO键盘
	while true
	do
		# clear 清场
		clear
		# 首先显示现在的内容
		# show ${TEXT[*]} ${INSERT[*]} $INSERTNUMBER
		eco "${TEXT[*]}" "${INSERT[*]}" "$INSERTNUMBER"

		# 其次空一行后开始读取输入内容
		echo ""
		read -e INSERT[$INSERTCOUNT]
		# 判断是否为：quit命令，如果是的话退出循环
		if [[ ${INSERT[$INSERTCOUNT]} == ":quit" ]]
		then
			unset INSERT[$INSERTCOUNT]
			break
		fi
		# 读取后INSERTCOUNT自增
		((INSERTCOUNT++))
	done
}


测试
TEXT=( hello world yes no )
LENGTH=4
insertSpecial 2 
echo ${TEXT[*]}

