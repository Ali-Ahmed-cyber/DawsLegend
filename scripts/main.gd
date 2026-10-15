extends Node3D

const PLAYER_SPEED := 7.0
const ENEMY_SPEED := 2.6
const ATTACK_RANGE := 2.2
const MAX_HEALTH := 100.0

var player: CharacterBody3D
var friends: Array[Node3D] = []
var enemies: Array[CharacterBody3D] = []
var mission_markers: Array[Node3D] = []
var health := MAX_HEALTH
var mission_index := 0
var score := 0
var boss_spawned := false
var boss_defeated := false
var game_completed := false
var attack_cooldown := 0.0
var message_timer := 0.0
var message_text := "الفصل الأول: بداية الحكاية"
var joystick_vector := Vector2.ZERO
var touch_move_origin := Vector2.ZERO
var touch_move_id := -1
var attack_requested := false
var jump_requested := false
var camera_pivot: Node3D
var camera: Camera3D
var hud: CanvasLayer
var health_bar: ProgressBar
var mission_label: Label
var message_label: Label
var score_label: Label
var mobile_stick: Control

func _ready() -> void:
	_build_world()
	_build_player()
	_build_companions()
	_build_enemies()
	_build_missions()
	_build_camera()
	_build_hud()
	_load_game()
	_set_message("الفصل الأول: بداية الحكاية. قابل توفيق وأمجد عند العلامة الخضراء.")

func _physics_process(delta: float) -> void:
	if not is_instance_valid(player):
		return
	attack_cooldown = maxf(0.0, attack_cooldown - delta)
	message_timer = maxf(0.0, message_timer - delta)
	if message_timer <= 0.0 and is_instance_valid(message_label):
		message_label.text = ""
	_move_player(delta)
	_update_camera(delta)
	_update_enemies(delta)
	_update_companions(delta)
	_check_missions()
	_update_hud()
	if attack_requested:
		_attack()
		attack_requested = false
	if jump_requested and player.is_on_floor():
		player.velocity.y = 6.5
		jump_requested = false

func _build_world() -> void:
	var env := WorldEnvironment.new()
	var environment := Environment.new()
	environment.background_mode = Environment.BG_SKY
	var sky := Sky.new()
	var sky_mat := ProceduralSkyMaterial.new()
	sky_mat.sky_top_color = Color(0.14, 0.29, 0.48)
	sky_mat.sky_horizon_color = Color(0.76, 0.69, 0.55)
	sky.sky_material = sky_mat
	environment.sky = sky
	environment.ambient_light_source = Environment.AMBIENT_SOURCE_COLOR
	environment.ambient_light_color = Color(0.78, 0.79, 0.82)
	environment.ambient_light_energy = 0.8
	env.environment = environment
	add_child(env)
	var sun := DirectionalLight3D.new()
	sun.rotation_degrees = Vector3(-48, -32, 0)
	sun.light_energy = 1.25
	add_child(sun)
	_make_box("Ground", Vector3(0, -0.35, 0), Vector3(180, 0.6, 180), Color(0.62, 0.53, 0.39), true)
	_make_box("Main asphalt road", Vector3(0, -0.02, 0), Vector3(14, 0.12, 180), Color(0.17, 0.18, 0.19), true)
	_make_box("Cross street", Vector3(0, 0.0, 0), Vector3(180, 0.14, 12), Color(0.19, 0.20, 0.21), true)
	for side in [-1, 1]:
		_make_box("Sidewalk", Vector3(side * 9.5, 0.08, 0), Vector3(5, 0.28, 180), Color(0.72, 0.67, 0.57), true)
		for i in range(9):
			var z := -72.0 + float(i) * 18.0
			var h := 7.0 + float((i * 7 + int(side)) % 5) * 1.8
			var w := 7.0 + float(i % 3) * 2.0
			var c := [Color(0.76,0.64,0.48), Color(0.65,0.57,0.46), Color(0.82,0.74,0.60)][i % 3]
			_make_building(Vector3(side * (17.0 + w * 0.5), h * 0.5, z), Vector3(w, h, 11.0), c, i, side)
	for i in range(24):
		var z := -82.0 + float(i) * 7.0
		_make_box("Road dash", Vector3(0, 0.08, z), Vector3(0.18, 0.025, 3.2), Color(0.95,0.87,0.59), false)
	for i in range(14):
		var x := -65.0 + float(i) * 10.0
		if absf(x) < 13.0:
			continue
		_make_box("Market awning", Vector3(x, 2.7, 4.0), Vector3(5.0, 0.25, 3.0), [Color(0.64,0.17,0.13),Color(0.12,0.39,0.29),Color(0.82,0.64,0.24)][i%3], true)
		_make_box("Market counter", Vector3(x, 1.0, 4.0), Vector3(4.2, 1.6, 1.6), Color(0.42,0.25,0.13), true)
	for i in range(18):
		var z := -78.0 + float(i) * 9.0
		_make_tree(Vector3(-13.0, 0.0, z))
		_make_tree(Vector3(13.0, 0.0, z + 3.0))

func _make_box(label: String, pos: Vector3, size: Vector3, color: Color, collision: bool) -> Node3D:
	var body: Node3D
	if collision:
		body = StaticBody3D.new()
	else:
		body = Node3D.new()
	body.name = label
	body.position = pos
	var mesh := MeshInstance3D.new()
	var box := BoxMesh.new()
	box.size = size
	mesh.mesh = box
	var mat := StandardMaterial3D.new()
	mat.albedo_color = color
	mat.roughness = 0.88
	mesh.material_override = mat
	body.add_child(mesh)
	if collision:
		var shape := CollisionShape3D.new()
		var box_shape := BoxShape3D.new()
		box_shape.size = size
		shape.shape = box_shape
		body.add_child(shape)
	add_child(body)
	return body

func _make_building(pos: Vector3, size: Vector3, color: Color, idx: int, side: int) -> void:
	_make_box("Building", pos, size, color, true)
	for floor_i in range(maxi(1, int(size.y / 2.4))):
		for col_i in range(2):
			var wx := pos.x - side * size.x * 0.24 + side * col_i * size.x * 0.48
			var wy := 1.4 + floor_i * 2.4
			if wy > size.y - 0.7:
				continue
			_make_box("Window", Vector3(wx, wy, pos.z - size.z * 0.5 - 0.065), Vector3(1.1, 1.0, 0.08), Color(0.20,0.42,0.55), false)
	if idx % 3 == 0:
		_make_box("Shop sign", Vector3(pos.x, 2.4, pos.z - size.z * 0.5 - 0.12), Vector3(3.4, 0.65, 0.12), Color(0.14,0.35,0.28), false)

func _make_tree(pos: Vector3) -> void:
	_make_box("Tree trunk", pos + Vector3(0,1.1,0), Vector3(0.55,2.2,0.55), Color(0.32,0.20,0.12), false)
	var crown := MeshInstance3D.new()
	var sphere := SphereMesh.new()
	sphere.radius = 1.35
	sphere.height = 2.7
	crown.mesh = sphere
	crown.position = pos + Vector3(0,2.8,0)
	var mat := StandardMaterial3D.new()
	mat.albedo_color = Color(0.15,0.34,0.18)
	crown.material_override = mat
	add_child(crown)

func _make_character(label: String, shirt: Color, is_player: bool) -> CharacterBody3D:
	var actor := CharacterBody3D.new()
	actor.name = label
	var capsule := CapsuleMesh.new()
	capsule.radius = 0.42
	capsule.height = 1.75
	var body_mesh := MeshInstance3D.new()
	body_mesh.mesh = capsule
	body_mesh.position.y = 1.1
	var mat := StandardMaterial3D.new()
	mat.albedo_color = shirt
	mat.roughness = 0.72
	body_mesh.material_override = mat
	actor.add_child(body_mesh)
	var head := MeshInstance3D.new()
	var head_mesh := SphereMesh.new()
	head_mesh.radius = 0.29
	head_mesh.height = 0.58
	head.mesh = head_mesh
	head.position.y = 2.12
	var skin := StandardMaterial3D.new()
	skin.albedo_color = Color(0.45,0.29,0.19)
	head.material_override = skin
	actor.add_child(head)
	var collider := CollisionShape3D.new()
	var shape := CapsuleShape3D.new()
	shape.radius = 0.42
	shape.height = 2.1
	collider.shape = shape
	collider.position.y = 1.05
	actor.add_child(collider)
	add_child(actor)
	return actor

func _build_player() -> void:
	player = _make_character("البطل - فلينة ريال مدريد البيضاء", Color(0.93,0.93,0.89), true)
	player.position = Vector3(0,0,12)
	player.set_meta("is_player", true)

func _build_companions() -> void:
	var tawfiq := _make_character("توفيق", Color(0.94,0.70,0.08), false)
	tawfiq.position = Vector3(-2,0,15)
	friends.append(tawfiq)
	var amjad := _make_character("أمجد", Color(0.12,0.12,0.14), false)
	amjad.position = Vector3(2,0,15)
	friends.append(amjad)

func _build_enemies() -> void:
	for i in range(8):
		var foe := _make_character("فرد العصابة %d" % (i+1), [Color(0.55,0.12,0.12),Color(0.28,0.18,0.42),Color(0.25,0.27,0.29)][i%3], false)
		foe.position = Vector3([-4.0,4.0,-6.0,6.0,-3.0,3.0,0.0,8.0][i],0,-8.0 - float(i)*5.5)
		foe.set_meta("health", 60.0)
		enemies.append(foe)

func _build_missions() -> void:
	var mission_positions := [Vector3(0,0,-8), Vector3(26,0,-38), Vector3(-24,0,-65), Vector3(-30,0,-72), Vector3(0,0,12)]
	var titles := ["الفصل 1: لقاء الشباب", "الفصل 2: حماية السوق", "الفصل 3: تطهير الحي", "الفصل 4: المواجهة الأخيرة", "الفصل 5: العودة بالنصر"]
	for i in range(mission_positions.size()):
		var marker := Node3D.new()
		marker.position = mission_positions[i]
		marker.set_meta("title", titles[i])
		var ring := MeshInstance3D.new()
		var torus := TorusMesh.new()
		torus.inner_radius = 1.05
		torus.outer_radius = 1.28
		ring.mesh = torus
		ring.position.y = 0.25
		var mat := StandardMaterial3D.new()
		mat.albedo_color = Color(0.15,0.95,0.60)
		mat.emission_enabled = true
		mat.emission = Color(0.04,0.5,0.22)
		ring.material_override = mat
		marker.add_child(ring)
		add_child(marker)
		mission_markers.append(marker)

func _build_camera() -> void:
	camera_pivot = Node3D.new()
	camera_pivot.position = Vector3(0,2.8,0)
	player.add_child(camera_pivot)
	camera = Camera3D.new()
	camera.position = Vector3(0,2.2,7.2)
	camera.rotation_degrees.x = -12
	camera.current = true
	camera_pivot.add_child(camera)

func _move_player(delta: float) -> void:
	var input_vec := Vector2.ZERO
	input_vec.x = Input.get_action_strength("move_right") - Input.get_action_strength("move_left")
	input_vec.y = Input.get_action_strength("move_back") - Input.get_action_strength("move_forward")
	input_vec += joystick_vector
	if input_vec.length() > 1.0:
		input_vec = input_vec.normalized()
	var direction := Vector3(input_vec.x,0,input_vec.y)
	if direction.length() > 0.05:
		player.velocity.x = direction.x * PLAYER_SPEED
		player.velocity.z = direction.z * PLAYER_SPEED
		player.rotation.y = lerp_angle(player.rotation.y, atan2(direction.x,direction.z), delta * 10.0)
	else:
		player.velocity.x = move_toward(player.velocity.x,0,PLAYER_SPEED*delta*5)
		player.velocity.z = move_toward(player.velocity.z,0,PLAYER_SPEED*delta*5)
	if not player.is_on_floor():
		player.velocity.y -= 18.0 * delta
	else:
		player.velocity.y = minf(player.velocity.y,0)
	player.move_and_slide()
	if Input.is_action_just_pressed("attack"):
		attack_requested = true
	if Input.is_action_just_pressed("jump"):
		jump_requested = true

func _update_camera(delta: float) -> void:
	var target := player.global_position + Vector3(0,2.5,0)
	camera.global_position = camera.global_position.lerp(target + Vector3(0,2.5,7.0), delta*3.5)
	camera.look_at(target, Vector3.UP)

func _update_enemies(delta: float) -> void:
	for foe in enemies:
		if not is_instance_valid(foe):
			continue
		var diff := player.global_position - foe.global_position
		var dist := diff.length()
		if dist < 22.0 and dist > 1.5:
			var dir := diff.normalized()
			foe.velocity.x = dir.x * ENEMY_SPEED
			foe.velocity.z = dir.z * ENEMY_SPEED
			foe.rotation.y = atan2(dir.x,dir.z)
		else:
			foe.velocity.x = move_toward(foe.velocity.x,0,delta*4)
			foe.velocity.z = move_toward(foe.velocity.z,0,delta*4)
		if not foe.is_on_floor():
			foe.velocity.y -= 18.0*delta
		else:
			foe.velocity.y = minf(foe.velocity.y,0)
		foe.move_and_slide()
		if dist < 1.65 and randf() < delta*0.7:
			health = maxf(0,health-5)
			if health <= 0:
				health = MAX_HEALTH
				player.position = Vector3(0,0,12)
				_set_message("وقعت في المواجهة ورجعت لنقطة البداية.")

func _update_companions(delta: float) -> void:
	for i in range(friends.size()):
		var friend := friends[i]
		if not is_instance_valid(friend):
			continue
		var target := player.global_position + Vector3((-1 if i==0 else 1)*1.7,0,2.0+float(i)*0.5)
		var d := target-friend.global_position
		if d.length() > 2.0:
			friend.velocity.x = d.normalized().x*5.0
			friend.velocity.z = d.normalized().z*5.0
		else:
			friend.velocity.x = move_toward(friend.velocity.x,0,delta*5)
			friend.velocity.z = move_toward(friend.velocity.z,0,delta*5)
		if not friend.is_on_floor():
			friend.velocity.y -= 18.0*delta
		else:
			friend.velocity.y = minf(friend.velocity.y,0)
		friend.move_and_slide()

func _attack() -> void:
	if game_completed:
		_set_message("انتهت القصة! تقدر تستكشف المدينة أو تبدأ من جديد.")
		return
	if attack_cooldown > 0.0:
		return
	attack_cooldown = 0.55
	var target: CharacterBody3D
	var best := ATTACK_RANGE
	for foe in enemies:
		if not is_instance_valid(foe):
			continue
		var d := player.global_position.distance_to(foe.global_position)
		if d < best:
			best = d
			target = foe
	if target:
		var hp := float(target.get_meta("health",60.0))-25.0
		target.set_meta("health",hp)
		if hp <= 0.0:
			if bool(target.get_meta("is_boss", false)):
				boss_defeated = true
			enemies.erase(target)
			target.queue_free()
			score += 100
			_set_message("هزمت خصماً! +100 نقطة")
			_save_game()
		else:
			_set_message("ضربة ناجحة! باقي للخصم %.0f صحة" % hp)
	else:
		_set_message("اقترب من الخصم قبل الضرب.")

func _check_missions() -> void:
	if game_completed or mission_index >= mission_markers.size():
		return
	var marker := mission_markers[mission_index]
	if player.global_position.distance_to(marker.global_position) >= 3.2:
		return
	if mission_index == 1 and enemies.size() > 5:
		_set_message("مهمة السوق: اهزم ثلاثة من أفراد العصابة أولاً.")
		return
	if mission_index == 2 and enemies.size() > 0:
		_set_message("الحي لسه ما آمن؛ اهزم بقية أفراد العصابة.")
		return
	if mission_index == 3 and not boss_defeated:
		if not boss_spawned:
			_spawn_boss()
		_set_message("زعيم العصابة ظهر! اهزمه لإنقاذ الحي.")
		return
	var completed_title := str(marker.get_meta("title"))
	mission_index += 1
	score += 250
	_save_game()
	_set_message("اكتملت المهمة: %s — +250 نقطة" % completed_title)
	if mission_index == 3 and not boss_spawned and not boss_defeated:
		_spawn_boss()
	if mission_index >= mission_markers.size():
		game_completed = true
		_set_message("النهاية: انتصرت مع توفيق وأمجد، ورجعتوا الحي بأمان. أسطورة دؤوس!")
		_save_game()
	elif mission_index == 1:
		_set_message("الفصل الثاني: العصابة تهدد السوق. اهزم ثلاثة من أفرادها ثم ارجع للعلامة.")
	elif mission_index == 2:
		_set_message("الفصل الثالث: أمّن الحي من بقية أفراد العصابة.")
	elif mission_index == 3:
		_set_message("الفصل الرابع: المواجهة الأخيرة. زعيم العصابة في انتظارك.")
	elif mission_index == 4:
		_set_message("الفصل الخامس: ارجع لنقطة البداية بعد الانتصار.")

func _spawn_boss() -> void:
	boss_spawned = true
	var boss := _make_character("زعيم العصابة", Color(0.16,0.055,0.06), false)
	boss.position = Vector3(-31,0,-73)
	boss.scale = Vector3(1.25,1.25,1.25)
	boss.set_meta("health", 240.0)
	boss.set_meta("is_boss", true)
	enemies.append(boss)
	_set_message("زعيم العصابة ظهر! صحته 240. استعد للمواجهة الأخيرة.")

func _save_game() -> void:
	var data := {
		"mission_index": mission_index,
		"score": score,
		"health": health,
		"player_position": [player.position.x, player.position.y, player.position.z],
		"boss_spawned": boss_spawned,
		"boss_defeated": boss_defeated,
		"game_completed": game_completed
	}
	var file := FileAccess.open("user://daws_legend_save.json", FileAccess.WRITE)
	if file:
		file.store_string(JSON.stringify(data))
		file.close()

func _load_game() -> void:
	if not FileAccess.file_exists("user://daws_legend_save.json"):
		return
	var file := FileAccess.open("user://daws_legend_save.json", FileAccess.READ)
	if not file:
		return
	var parsed = JSON.parse_string(file.get_as_text())
	file.close()
	if typeof(parsed) != TYPE_DICTIONARY:
		return
	mission_index = clampi(int(parsed.get("mission_index", 0)), 0, mission_markers.size())
	score = int(parsed.get("score", 0))
	health = clampf(float(parsed.get("health", MAX_HEALTH)), 1.0, MAX_HEALTH)
	boss_spawned = bool(parsed.get("boss_spawned", false))
	boss_defeated = bool(parsed.get("boss_defeated", false))
	game_completed = bool(parsed.get("game_completed", false))
	var saved_position = parsed.get("player_position", [0.0,0.0,12.0])
	if saved_position is Array and saved_position.size() == 3:
		player.position = Vector3(float(saved_position[0]),float(saved_position[1]),float(saved_position[2]))
	if boss_spawned and not boss_defeated:
		_spawn_boss()
	elif boss_defeated:
		boss_spawned = true

func _build_hud() -> void:
	hud = CanvasLayer.new()
	add_child(hud)
	var panel := PanelContainer.new()
	panel.position = Vector2(18,18)
	panel.custom_minimum_size = Vector2(340,126)
	hud.add_child(panel)
	var layout := VBoxContainer.new()
	layout.add_theme_constant_override("separation",6)
	panel.add_child(layout)
	var title := Label.new()
	title.text = "DAWS LEGEND  |  أسطورة دؤوس"
	title.add_theme_font_size_override("font_size",22)
	layout.add_child(title)
	health_bar = ProgressBar.new()
	health_bar.max_value = MAX_HEALTH
	health_bar.value = health
	health_bar.custom_minimum_size.y = 18
	layout.add_child(health_bar)
	mission_label = Label.new()
	mission_label.text = "المهمة: قابل الشباب"
	layout.add_child(mission_label)
	score_label = Label.new()
	score_label.text = "النقاط: 0  |  الأعداء: %d" % enemies.size()
	layout.add_child(score_label)
	message_label = Label.new()
	message_label.position = Vector2(360,22)
	message_label.custom_minimum_size = Vector2(560,52)
	hud.add_child(message_label)
	_build_mobile_controls()

func _build_mobile_controls() -> void:
	var move_pad := Control.new()
	move_pad.name = "MobileMovePad"
	move_pad.set_anchors_preset(Control.PRESET_BOTTOM_LEFT)
	move_pad.position = Vector2(28,-205)
	move_pad.size = Vector2(170,170)
	move_pad.gui_input.connect(_on_move_pad_input)
	hud.add_child(move_pad)
	var pad_bg := ColorRect.new()
	pad_bg.color = Color(0.08,0.12,0.13,0.45)
	pad_bg.size = Vector2(150,150)
	pad_bg.position = Vector2(10,10)
	move_pad.add_child(pad_bg)
	var pad_label := Label.new()
	pad_label.text = "تحريك"
	pad_label.position = Vector2(48,65)
	pad_label.add_theme_font_size_override("font_size",20)
	move_pad.add_child(pad_label)
	var attack_btn := Button.new()
	attack_btn.text = "ضرب"
	attack_btn.set_anchors_preset(Control.PRESET_BOTTOM_RIGHT)
	attack_btn.position = Vector2(-150,-150)
	attack_btn.size = Vector2(112,78)
	attack_btn.pressed.connect(func(): attack_requested = true)
	hud.add_child(attack_btn)
	var jump_btn := Button.new()
	jump_btn.text = "قفز"
	jump_btn.set_anchors_preset(Control.PRESET_BOTTOM_RIGHT)
	jump_btn.position = Vector2(-265,-90)
	jump_btn.size = Vector2(90,60)
	jump_btn.pressed.connect(func(): jump_requested = true)
	hud.add_child(jump_btn)
	var save_btn := Button.new()
	save_btn.text = "حفظ"
	save_btn.set_anchors_preset(Control.PRESET_TOP_RIGHT)
	save_btn.position = Vector2(-118,18)
	save_btn.size = Vector2(100,48)
	save_btn.pressed.connect(func(): _save_game(); _set_message("تم حفظ تقدمك على الجهاز."))
	hud.add_child(save_btn)
	var reset_btn := Button.new()
	reset_btn.text = "إعادة"
	reset_btn.set_anchors_preset(Control.PRESET_TOP_RIGHT)
	reset_btn.position = Vector2(-228,18)
	reset_btn.size = Vector2(100,48)
	reset_btn.pressed.connect(_reset_game)
	hud.add_child(reset_btn)

func _reset_game() -> void:
	var file := FileAccess.open("user://daws_legend_save.json", FileAccess.WRITE)
	if file:
		file.store_string("{}")
		file.close()
	get_tree().reload_current_scene()

func _on_move_pad_input(event: InputEvent) -> void:
	if event is InputEventScreenTouch:
		if event.pressed:
			touch_move_id = event.index
			_update_stick(event.position)
		else:
			if event.index == touch_move_id:
				touch_move_id = -1
				joystick_vector = Vector2.ZERO
	elif event is InputEventScreenDrag and event.index == touch_move_id:
		_update_stick(event.position)

func _update_stick(pos: Vector2) -> void:
	var center := Vector2(85,85)
	var offset := pos-center
	joystick_vector = Vector2(clampf(offset.x/55.0,-1,1),clampf(offset.y/55.0,-1,1))

func _update_hud() -> void:
	if is_instance_valid(health_bar):
		health_bar.value = health
	if is_instance_valid(mission_label):
		mission_label.text = "المهمة: %s" % ("اكتملت كل المهام" if mission_index>=mission_markers.size() else str(mission_markers[mission_index].get_meta("title")))
	if is_instance_valid(score_label):
		score_label.text = "النقاط: %d  |  الأعداء: %d" % [score,enemies.size()]

func _set_message(text: String) -> void:
	message_text = text
	message_timer = 4.5
	if is_instance_valid(message_label):
		message_label.text = text
