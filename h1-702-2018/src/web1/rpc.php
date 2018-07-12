<?php

// define error constants
define('ERROR_400', 'HTTP/1.1 400 Bad Request');
define('ERROR_401', 'HTTP/1.1 401 Unauthorized');
define('ERROR_404', 'HTTP/1.1 404 Not Found');
define('ERROR_406', 'HTTP/1.1 406 Unacceptable');
define('ERROR_415', 'HTTP/1.1 415 Unsupported Media Type');

// define ok response codes
define('OK_200', 'HTTP/1.1 200 OK');
define('OK_201', 'HTTP/1.1 201 Created');

// define flag constants
define('FLAG', base64_encode('702-CTF-FLAG: NP26nDOI6H5ASemAOW6g'));
define('FLAG_KEY', 'EelHIXsuAw4FXCa9epee');
define('FLAG_TIME', 1528911533);

// define user IDs
define(USERS, [1, 2]);

// define a respond_with helper
function respond_with($header, $body) {
  header($header);
  header('Content-Type: application/json');

  die(json_encode($body));
}

// make database unique to requester and the authenticated user
function noteFileName($id) {
  $ip = $_SERVER['REMOTE_ADDR'];

  return '/tmp/notes_' . $ip . '_' . $id . '.csv';
}

// return array of notes objects
// read the CSV file and dump the data in an object
function notes($id) {
  $notes = [];

  if(file_exists(noteFileName($id))) {
    $lines = explode("\n", file_get_contents(noteFileName($id)));

    foreach($lines as $line) {
      $data = explode(',', $line);

      $notes[$data[0]] = [
        'note' => $data[1],
        'epoch' => $data[2],
      ];
    }
  }

  return $notes;
}

// helper method to define the format of the CSV
function noteDataLine($id, $note, $epoch) {
  return $id . ',' . $note . ',' . $epoch;
}

// helper method to return a header
function getHeader($header) {
  foreach(apache_request_headers() as $key => $value) {
    if(strtolower($key) === $header) {
      return $value;
    }
  }

  return;
}

function acceptHeader() {
  return getHeader('accept');
}

function contentTypeHeader() {
  return getHeader('content-type');
}

function authorizationHeader() {
  return getHeader('authorization');
}

//
function writeNoteData($id, $note_id, $note, $time, $version) {
  $notes = notes($id);

  $notes[$note_id] = [
    'note' => $note,
    'epoch' => $time,
  ];

  $contents = '';

  // sorting introduces the ability to determine the hidden key
  // based on the epoch sort in the getNotesMetadata endpoint
  if($version === 2) {
    ksort($notes);
  }

  foreach($notes as $key => $data) {
    $contents .= noteDataLine($key, $data['note'], $data['epoch']) . "\n";
  }

  $contents = rtrim($contents, "\n"); // strip last new line

  file_put_contents(noteFileName($id), $contents);
}

// depending on the user ID, reset the note data
function resetNoteData($id) {
  if(file_exists(noteFileName($id))) {
    unlink(noteFileName($id));
  }

  if($id === 1) {
    writeNoteData($id, FLAG_KEY, FLAG, FLAG_TIME, 1);
  }
}

// check version header
if(acceptHeader() !== null) {
  if(acceptHeader() === 'application/notes.api.v1+json') {
    $version = 1;
  } elseif(acceptHeader() === 'application/notes.api.v2+json') {
    $version = 2;
  } else {
    respond_with(ERROR_415, ['version' => 'is unknown']);
  }
} else {
  respond_with(ERROR_415, ['version' => 'is missing']);
}

// authentication
if(authorizationHeader() !== null) {
  $auth = (string)authorizationHeader();

  $jwt = explode('.', $auth);

  if(count($jwt) === 3) {
    $algorithm = json_decode(base64_decode($jwt[0]));
    $data = json_decode(base64_decode($jwt[1]));
    $signature = $jwt[2];

    if(!is_object($algorithm) || !is_object($data)) {
      respond_with(ERROR_401, ['authorization' => 'is invalid']);
    }

    if(array_key_exists('id', $data)) {
      $id = (int)$data->id;
    } else {
      respond_with(ERROR_401, ['authorization' => 'is invalid']);
    }

    if(array_key_exists('alg', $algorithm)) {
      $used_algorithm = strtolower((string)$algorithm->alg);
    } else {
      $used_algorithm = 'HS256';
    }

    if($id === 2 && ($signature !== 't4M7We66pxjMgRNGg1RvOmWT6rLtA8ZwJeNP-S8pVak' && $used_algorithm !== 'none')) {
      respond_with(ERROR_401, ['authorization' => 'is invalid']);
    }

    if($id === 1 && $used_algorithm !== 'none') {
      respond_with(ERROR_401, ['authorization' => 'is invalid']);
    }

    if(!in_array($id, USERS)) {
      respond_with(ERROR_401, ['authorization' => 'is invalid']);
    }
  } else {
    respond_with(ERROR_401, ['authorization' => 'is invalid']);
  }
} else {
  respond_with(ERROR_401, ['authorization' => 'is missing']);
}

// set up database file if user doesn't have one
// same as deleteNotes endpoint
if(!file_exists(noteFileName($id))) {
  resetNoteData($id);
}

// determine RPC method and handle request
$rpc = $_GET['method'];

if($rpc === 'getNotesMetadata' && $_SERVER['REQUEST_METHOD'] === 'GET') {
  $epochs = [];

  foreach(notes($id) as $key => $data) {
    $epochs[] = $data['epoch'];
  }

  respond_with(OK_200, ['count' => count(notes($id)), 'epochs' => $epochs]);
} elseif($rpc === 'getNote' && $_SERVER['REQUEST_METHOD'] === 'GET') {
  $note_id = (string)$_GET['id'];

  if(!array_key_exists($note_id, notes($id))) {
    respond_with(ERROR_404, ['note' => 'is not found']);
  } else {
    $note = notes($id)[$note_id];

    respond_with(OK_200, $note);
  }
} elseif($rpc === 'createNote' && $_SERVER['REQUEST_METHOD'] === 'POST') {
  if(strtolower(contentTypeHeader()) !== 'application/json') {
    respond_with(ERROR_406, ['data' => 'is invalid']);
  }

  $body = file_get_contents('php://input');
  $decoded_body = json_decode($body);

  if(!is_object($decoded_body)) {
    respond_with(ERROR_400, ['data' => 'is invalid']);
  }

  if(array_key_exists('id', $decoded_body)) {
    $note_id = (string)$decoded_body->id;
  } else {
    $note_id = md5(openssl_random_pseudo_bytes(16));
  }

  if(array_key_exists($note_id, notes($id))) {
    respond_with(ERROR_400, ['note' => 'already exists']);
  }

  $note = (string)$decoded_body->note;

  // do some input validation for the body
  if(preg_match("/\A[a-zA-Z0-9\s\.\-]+\z/", $note) === 0) {
    respond_with(ERROR_400, ['note' => 'is invalid']);
  }

  if(preg_match("/\A[a-zA-Z0-9]+\z/", $note_id) === 0) {
    respond_with(ERROR_400, ['id' => 'is invalid']);
  }

  // store file
  writeNoteData($id, $note_id, $note, time(), $version);

  // redirect to the note
  respond_with(OK_201, ['url' => '/rpc.php?method=getNote&id=' . $note_id]);
} elseif($rpc === 'resetNotes' && $_SERVER['REQUEST_METHOD'] === 'POST') {
  resetNoteData($id);

  respond_with(OK_200, ['reset' => true]);
} else {
  respond_with(ERROR_404, ['method' => 'not found']);
}
