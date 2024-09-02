package com.music.service;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import com.music.dao.CollectionDao;

public class CollectionService {

	public List<Map<String, Object>> getCollection(int uid) throws SQLException {
		CollectionDao dao = new CollectionDao();
		List<Map<String, Object>> list = dao.getCollection(uid);
		return list;
	}
}
