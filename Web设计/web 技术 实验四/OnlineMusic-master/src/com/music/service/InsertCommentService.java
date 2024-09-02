package com.music.service;

import java.sql.SQLException;

import com.music.dao.InsertCommentDao;
import com.music.domain.SongComment;

public class InsertCommentService {

	public boolean insertComment(SongComment comment) throws SQLException {
		InsertCommentDao dao = new InsertCommentDao();
		boolean flag = dao.InsertComment(comment);
		return flag;
	}

}
